// Author: Alyssa Kelley
// Assignment 4 Part 3

#include <stdio.h>

#define N 4

void swap(long *xp, long *yp)
{
/* This function swaps the int values stored at the xp and yp pointers. 
This function was adapted from the powerpoint lecture in class. 
Source - CIS 314 PPW 03, Slide 17 */
	long t1 = *xp; /* The value t1 is set to pointer xp. */
	long t2 = *yp; /* The value t2 is set to the pointer yp. */
	*xp = t2; /* The xp pointer is now changed to t2 which held pointer yp */
	*yp = t1; /* The yp pointer is now changed to t1 which held pointer xp */
}

typedef long array_t[N][N];

void old_transpose(array_t a) 
{
	for (long i = 0; i < N; ++i) 
	{
		for (long j = 0; j < i; ++j) 
		{
			long t1 = a[i][j];
			long t2 = a[j][i];
			a[i][j] = t2;
			a[j][i] = t1;
		}
	}
}
/*

.L3: Initalizing loop.
	cmpq %rcx, %rsi 	-> compare N src (rcx) to N dest (rsi) -> rsi - rcx -> j - i

	jle .L7 			-> jump to L7 loop if that compare if (rsi -rcx) is less than 1.

	movq (%rdx), %r9 	-> t1 pointer is at index i (r9)

	movq (%rax), %r8 	-> t2 pointer is at index j (r8)

	movq %r9, (%rax)	-> moving (swapping i to j) t1 pointer to j 

	movq %r8, (%rdx) 	-> moving (swapping j to i) t2 pointer to i

	addq $8, %rax 		-> offset the j poitner by 8 (1 -> ++)

	addq $32, %rdx 		-> offset the i pointer by 32 (4 -> N)

	addq $1, %rcx 		-> incrementing the pointer by 1 (either i or j)

	jmp .L3 			-> jump to the beginnging of the L3 loop to repeat.

*/
void transpose(array_t a)
{

	long temporary_holder;

	for(long i = 0; i < N; ++i)  // same as original given function.
	{
		 long *xp = &a[i][0]; // columns
		 long *yp = &a[0][i]; // rows

		for (long j = 0; j < i; ++j) // same as original given function.
		{	
			swap(xp, yp); /* calling swap to change the positions of t1 and t2, so now 
			The xp pointer is now changed to t2, and the yp pointer is now changed to t1. */
			xp ++; // Increamenting up. 
			yp += N; // N because it is 4. 
		}
	}

	printf("{ ");
	for (int i = 0; i < N; ++i)
	{
		printf("{");

		for (int j = 0; j < N; ++j)
		{
			if (j < N-1)  // Only printing a comma after the number if it is not the last one in the sub arrays.
			{
				printf("%li, ", a[i][j]);
			}
			else
			{
				printf("%li ", a[i][j]);	
			}
		}

		if (i < N-1)
		{
			printf("}, ");	// Only printing a comma after each inner array if it is not the last array within the big array.
		}
		else
		{
			printf("} ");
		}
	}

	printf("}\n");
}

int main()
{
	array_t a = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	transpose(a);  
	// this should print out: {{1, 5, 9, 13}, {2, 6, 10, 14}, {3, 7, 11, 15}, {4, 8, 12, 16}}
	return 0;
}