// Author: Alyssa Kelley
// Project: Assignment 3 Part 1

#include <stdio.h>
#include <stdlib.h>

struct IntArray {
	int length;
	int *dataPtr;
};

struct IntArray* mallocIntArray(int length){
/* This struct is designed to allocate, initialize, and return a
pointer to a new struct IntArray. This has two malloc calls, 
one is for the instance and one for the instance's dataPtr (Note:
the dataPtr is a pointer to an int array of size length)
*/
	struct IntArray* new_iA = (struct IntArray*) malloc(sizeof(struct IntArray)); 
		/* This is casting the data type to be struct, and then allocating the memory for 
		the struct IntArray */
	new_iA -> length = length; /* This is dereferencing the pointer to get inside the 
		struct IntArray and access that length. This can also be done by doing (*new_iA).len */
	new_iA -> dataPtr = (int*) malloc(sizeof(int) * length); /* This is allocating new
		memory for the dataPtr, and multiplying by the length will give you the correct amount 
		of integers for that dataPtr array. */
	for(int i = 0; i < length; i++){
		new_iA -> dataPtr[i] = 0; /* This will replace all the null memory in the 
			dataPtr array to 0's. If we pointed to something that had null memory,
			this would give us an error. */
	}
	return new_iA; /* returing the pointer to the new struct IntArray (new_iA) */
}

void freeIntArray(struct IntArray *arrayPtr){ /* Passing in what you are wanting to free */
/* This function will free the instance's dataPtr and frees the instance */
	if ((arrayPtr != NULL) && ((arrayPtr -> dataPtr) != NULL)){ /* This is making sure
		that what you are working with is NOT null. */
		for (int i = 0; i < arrayPtr -> length; i++){
			free(arrayPtr -> dataPtr); /* free is a built in function. Free take 
				in a pointer and clears it out entirely(but not putting a 0 there.) 
				The main goal with this is so that the heap does not continue to
				increase and the stack hits it. */
			arrayPtr -> dataPtr = 0;
			arrayPtr -> length = 0;
			}
		free(arrayPtr);
		} 
	else
		exit(1); /* It will break if the memory is null */	
}

int readInt(int* data)
{
	/* Sources: I used the different concepts provided in the following sources to create the content of
	this function:
	https://www.tutorialspoint.com/c_standard_library/c_function_getchar.htm
	https://www.geeksforgeeks.org/fgets-gets-c-language/
	https://www.geeksforgeeks.org/strtol-function-in-c-stl/
	I also worked on this in my study group with Anne G., Kristine S., and Miguel to work through
	this. 
	*/
	int valid, temporary;
	valid = scanf("%d", data); /* This is using the built in scan
	function to look for only integers to store in data and only
	scanning for them 1 at a time. If you get anything more than 
	a single integer, it will not give you 1, so this would 
	indicate it is invalid. */
	if (valid != 1)
	{ /* Indicating it is not a valid entry */
		while (((temporary = getchar()) != EOF) && (temporary != '\n'));
		/* This is clearning the buffer, acting like a flush. This is the entire while loop for this in one
		line. getchar() goes through each character until it is EOF or a new line*/
		printf("Invalid input\n");
		return 0; /* Returning False */
	}
	else
	{
		return 1;
		 /* Indicating a valid entry */
	}
}

void readIntArray(struct IntArray *array){
/* This function prompts and reads ints from the user to fill the array. If 
the user enters a value that cannot be parsed as an int, then an error message 
and prompt is printed.
I also worked on this in my study group with Anne G., Kristine S., and Miguel to work 
through this.
*/
	int valid = 0; /* This is setting it as 0 to create a boolean indicating False. */
	for (int i = 0; i < (array -> length); i++){
		valid = 0;
		while(valid != 1){ /* While the input is True, do the following */
			printf("Enter int: ");
			valid = readInt(&(array -> dataPtr[i]));
		}
	}
}

void swap(int *xp, int *yp)
{
/* This function swaps the int values stored at the xp and yp pointers. 
This function was adapted from the powerpoint lecture in class. 
Source - CIS 314 PPW 03, Slide 17 */
	int t0 = *xp; /* The value t0 is set to pointer xp. */
	int t1 = *yp; /* The value t1 is set to the pointer yp. */
	*xp = t1; /* The xp pointer is now changed to t1 which held pointer yp */
	*yp = t0; /* The yp pointer is now changed to to which held pointer xp */
}

void sortIntArray(struct IntArray *array){
/* This function sorts the input array in ascending order using Selection Sort. 
Here is the source for where I read about some of the logic used for creating 
this function: 
Geeksforgeeks coding example found here: https://www.geeksforgeeks.org/selection-sort/ 
This is sorting through the array by comparing the two values and sorting them 
as it continues on through the array, and this is why we had to use a nested for loop,
so it will loop through the entire array until all of the values are sorted for that
length.
*/
	int i, j, min_index;
	for (i = 0; i < ((array -> length) - 1); i++){
		min_index = i;
		for (j = i+1; j < (array -> length); j++){
			if ((array -> dataPtr[j]) < (array -> dataPtr[min_index])){
				min_index = j;
			}
		}
		swap(&array -> dataPtr[min_index], &array -> dataPtr[i]);
	}
}

void printIntArray(struct IntArray *array){
/* This function will print the array in square braclets in assending order 
seperated by commans. I did an if and else statement so the last number in 
the array would not print an additional comma after that length-1 value 
prints out. I also included a space before the first int after the first 
brace, and after the last int before the last brace since this is how the output
was printed in the project specifications.*/
	printf("["); /* This is starting off the squared bracket for the array */
	for (int i=0; i < (array -> length); i++){ /* This is looping through the length 
		of the array. */
		if (i == (array -> length-1))
		{
			printf(" %d ", array -> dataPtr[i]);
		}
		else
		{
			printf(" %d,", array -> dataPtr[i]); /* Printing the next (lowest) int in the ordered array. */
		}
	}
	printf("]\n"); /* Ending the array with the final square bracket and then going to a new line. */
}


int main(int argc, char *argv[]){
/* The main function will prompt the user to input a positive int length from the 
user (print an error message and prompt again if the input cannot be parsed
as a positve int). This function calls mallocIntArray to create the array, calls
readIntArray to prompt the user to fill the array, call sortIntArray to sorit it, 
call printArray to prompt the user to fill the array, call sortArray to sory it, 
call printArray to print the resulting sorted array, then call freeIntArray to
free the heap memory used by the array.

Note: The arguments are argc which is the amount of the arguement, and argv which is
the arguement. This naming convention is standard.
*/
	int length = 0;
	int valid = 0; /* valid = False */
	while (valid != 1){ /* While(Valid=True) */
		printf("Enter length: ");
		valid = readInt(&length); /* This goes into the argv location to make
		the array this length that the user inputted. */
	}
	struct IntArray *myArray = mallocIntArray(length);
	readIntArray(myArray); /* You do not need to continue to put *myArray
		in the following function calls since they are already expecting 
		a pointer input. */
	sortIntArray(myArray);
	printIntArray(myArray);
	freeIntArray(myArray);
	return 0; /* Returning 0 to indicate funciton success. */
}

/* Note: an example test case: 
Enter length: cat 
Invalid input 
Enter length: 5 
Enter int: 3 
Enter int: puppy 
Invalid input 
Enter int: 7 
Enter int: 5
Enter int: 8 
Enter int: 2 
[ 2, 3, 5, 7, 8 ]
*/
