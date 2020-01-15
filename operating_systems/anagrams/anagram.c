/*

Authorship Statement

Name: Alyssa Kelley 
DUCK ID: alyssak
CIS 415 Project 0

This is my own work and absolutly no code was copied. I did discuss this project with Anne Glickenhaus (classmate), Luke Peyralans (fellow CS student at UO), and Zack Bower (fellow CS student at UO). 

I also used the following links to better my understanding for concepts such as mallocing and linked lists:

https://dev-notes.eu/2018/07/double-pointers-and-linked-list-in-c/

https://stackoverflow.com/questions/7271647/what-is-the-reason-for-using-a-double-pointer-when-adding-a-node-in-a-linked-lis/7271680#7271680

https://www.hackerearth.com/practice/data-structures/linked-list/singly-linked-list/tutorial/

https://www.youtube.com/watch?v=v49bwqQ4ouM

*/

#include "anagram.h"

struct StringList * MallocSList(char *word)
{
	/* This function should malloc for the StringList struct, 
	and a new string list node is created in this function,
	and returned. */

	int string_length = strlen(word);

	struct StringList *next_string_list = malloc (sizeof(struct StringList)); // allocating memory for the entire struct

	next_string_list -> Word = malloc(string_length + 1); // sizeof(char) = 1 so ommitted.

	int i;
	for (i = 0; i < (string_length + 1); i++)
	{
		next_string_list -> Word[i] = 0; // initialize the characters in the next word to all be 0
	}

	strcpy(next_string_list -> Word, word); // Saving the word passed in, in the struct. Recall: strcpy(dest, source)

	(next_string_list -> Word)[string_length] = '\0';

	next_string_list -> Next = NULL;

	return next_string_list; // returning the pointer to the struct
}

void AppendSList(struct StringList **head, struct StringList *node)
{
	/* Append a string list node to the end/tail of a string list.
	head is the beginning of the list which is the first node aka 
	the first word in the dict and the node is the the next word 
	in that dictionary. */

	//printf("In AppendSList!\n");
	struct StringList * current = *head; // address of head pointer (??)

	//printf("just set current to the head passed in\n");
	//printf("This is the word in the current node %s \n", current -> Word);

	while(current -> Next != NULL) // loop to the end of the StringList
	{
		//printf("This node is not null");
		current = current -> Next; // going next
	}
	current -> Next = node;
}

void FreeSList(struct StringList **node)
{ 
	/* Free a string list, including all children. */

	struct StringList * current_node;

	current_node = *node; // address in node pointer 

	struct StringList * next_node;

	while (current_node != NULL) // loop through each node
	{
		free(current_node  -> Word); // and free the word
		next_node = current_node -> Next;

		free(current_node);
		current_node = next_node;
	}

	free(current_node);
}

void PrintSList(FILE *file, struct StringList *node)
{
	/* Format output to a file according to specification. */

	while(node != NULL) // For each node in the StringList
	{
		//printf("** %s **\n", node -> Word);

		fprintf(file, "\t%s\n", node -> Word); 

		node = node -> Next; // Goes to the next node so then when it loops around again, it is the next node aka the next word
	}
}

int SListCount(struct StringList *node)
{
	/* Return the number of strings in the string list. */
	int counter = 0;

	while(node != NULL) // Loop through the entire StringList
	{
		counter++; // updating the counter each time we  get another word in the StringList
		node = node -> Next;
	}

	return counter; // Return the count value for the number for words in the list

}

char * uncapitilize_word(char * word, int string_length)
{
	/* This function will uncapitalize a given word. */

	int i;
	for (i = 0; i < (string_length + 1); i++)
	{
		word[i] = (tolower(word[i])); // built in with ctype.h
	}

	return word;

}


char * sort_the_word(char * word, int string_length)
{
	/* This function will sort the word in alphabetical order after it was uncapitalized. 
	(site: https://www.tutorialspoint.com/learn_c_by_examples/program_to_sort_string_characters_in_c.htm) */

	char temp_letter;

	int i;
	for (i = 0; i < string_length - 1; i++)
	{
		int j;
		for (j = i + 1; j < string_length; j++)
		{
			if (word[i] > word[j])
			{
				temp_letter = word[i];
				word[i] = word[j];
				word[j] = temp_letter;
			}
		}
	}

	return word;
}

struct AnagramList* MallocAList(char *word)
{
	/* Create a new anagram node, including the string list node with the word. */
	int string_length = strlen(word);

	struct AnagramList * new_A_List= malloc(sizeof(struct AnagramList)); // Allocate the memory for AnagramList struct

	new_A_List -> Anagram = malloc(string_length + 1);

	struct StringList * new_S_List = 0;

	new_S_List = MallocSList(word);

	char * copy_of_word;

	copy_of_word = strdup(word); // making a copy of the word and strdup mallocs for us

	uncapitilize_word(copy_of_word, string_length); // calling the function I wrote to uncapitalize the word and return it

	//printf("** This is the copy of the word: %s **\n", copy_of_word);

	sort_the_word(copy_of_word, string_length); // calling the sort function I wrote which implements swapping

	//printf("** This is the sorted word: %s **\n", copy_of_word);

	strcpy(new_A_List -> Anagram, copy_of_word);

	//printf("** Just saved %s in the Anagram struct as %s **\n", copy_of_word, new_A_List -> Anagram);

	new_A_List -> Words = new_S_List; // trying to malloc for the StringList of words for this anagram
	new_A_List -> Next = NULL; // set next to null

	free(copy_of_word);

	return new_A_List; // return the pointer to the struct for Anagrams
}

char * get_anagram_name(char * word, int string_length)
{
	/* This function will take a word, and find it's anagram name for that word.
	The anagram name is going to serve in place of the root anagram this word belongs to,
	and that is the uncapitalized, unsorted version of the word. */

	char * anagram_name;

	anagram_name = uncapitilize_word(word, string_length);

	anagram_name = sort_the_word(word, string_length);

	return anagram_name;
}

void AddWordAList(struct AnagramList **node, char *word)
{
	/* This function is adding a new word to an anagram list and performs this task with 
	three different scenarios: 1) The node is null, so malloc. 2) If the copy matches the
	anagram, then call the two functions mallocSList and appendSList. 3) It did not match
	any anagram, therefore it is a new one, so call MallocAList and add it to the anagram 
	list. */

	if (*node == NULL) // dereference to get the address of the AnagramList
	{
		// if it is NULL, then we have nothing to search through, and we need to malloc for the anagram list
		*node = MallocAList(word);

		return; // done for now, exit the function
	}

	char * anagram_name; 

	char * copy_of_anagram_name;

	copy_of_anagram_name = strdup(word); // casting

	int string_length = strlen(word);

	anagram_name = get_anagram_name(copy_of_anagram_name, string_length); // This function is getting a copy of the word, and this
	// is going to be used as the anagram name which we are calling the family title.

	//printf("This is the anagram name: %s\n", anagram_name);

	struct AnagramList *current = 0;
	current = *node; // creates a copy of the node to be at the "current" position

	struct AnagramList *last = 0; // keep track of last position in the list of anagrams.

	while(current != NULL) // loop through the existing anagram list
	{
		last = current; // keeping track of the last position to match where we currently are in the list

		if (strcmp(anagram_name, current -> Anagram) == 0) 
		{
			struct StringList * new_S_List = 0; // initializing the new string list
			new_S_List = MallocSList(word); // call MallocSList and pass in the original word

			AppendSList(&(current -> Words), new_S_List); // call appendSList with the new StringList node

			// clean up
			free(copy_of_anagram_name);
			//printf("** %s is already part of our AnagramList **\n", anagram_name);
			return; // done for now, exit the function
		}

		current = current -> Next; // updating the current position to continue iterating over the anagram list
	}

	struct AnagramList * new_A_List = 0; // this will be at the end, if the two other scenarios never exit
	// out of the function, then that means we searched through all the anagrams, and there is not a match
	// for this anagram, so we need to add it to the end of the list.

	new_A_List = MallocAList(word); // call MallocAlist(word);

	last -> Next = new_A_List; // we use the last Anagram list variable which should be the last position
	// from when we did our while loop since we were updating it with the current position.

	free(copy_of_anagram_name);

	return; // done - added the new anagram to the end of the list, exit the function

}


void PrintAList(FILE *file,struct AnagramList *node)
{
	/* Format output to a file, print anagram list with words, according to spec */

	int number_of_anagrams = 0;

	while(node != NULL) // For each node/Anagram in the AnagramList
	{
		number_of_anagrams = SListCount(node -> Words); // using the StringList from the AnagramList struct passed in called node

		if (number_of_anagrams >= 2)
		{
			fprintf(file, "%s:%d\n", node -> Anagram, number_of_anagrams); // printing the anagram:number
			PrintSList(file, node -> Words); // calling PrintSList which will print the words that match this anagram
		}

		node = node -> Next; // goes to the next node so then when it loops around again, it is the next node aka the next word
	}
}

void FreeAList(struct AnagramList **node)
{
	/* Free an anagram list, including anagram children and string list words. */

	struct AnagramList * current_A_node;

	current_A_node = *node;

	struct AnagramList * next_A_node;

	while(current_A_node != NULL) // for each Anagram in Alist
	{
		FreeSList(&current_A_node -> Words); // call FreeSList

		free(current_A_node -> Anagram);

		next_A_node = current_A_node -> Next; // increment the node_pointer

		free(current_A_node); // free the AnagramList

		current_A_node = next_A_node;
	}
}
