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
int main(int argc, char* argv[])
{
	FILE *input;
	FILE *output;

	switch(argc)
	{
		case 1: 
			input = stdin;
			output = stdout;
			break;
		case 2:
			input = fopen(argv[1], "r");
			output = stdout;
			break;
		case 3:
			input = fopen(argv[1], "r");
			output = fopen(argv[2], "w");
			break;
		default:
			printf("gcc <program_ex> <file1> <file2>\n");
			exit(1);
	}

	if (input == NULL || output == NULL)
	{
		printf("Error opening the files. Exiting program.\n");
		exit(1);
	}

	int BUFFER_SIZE = 1024;

	char line[BUFFER_SIZE];

	struct AnagramList * current_anagram = NULL;

	while(fgets(line, sizeof(line), input) != NULL)
	{
		line[strlen(line) - 1] = '\0'; // gets rid of new line
		//printf("In while loop main and this is the line value [* %s *]\n", line);
		AddWordAList(&current_anagram, line);
	}

	PrintAList(output, current_anagram); // Print anagram list
	FreeAList(&current_anagram); // Free anagram list

	fclose(input); // close input file
	fclose(output); // close output file

	return 0;
}