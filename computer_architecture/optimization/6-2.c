// Author: Alyssa Kelley
// Assignment 6 - Part 2c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 20000 // Length of array

/*
# u in %rbx, v in %rax, length in %rcx, i in %rdx, sum in %xmm1
.L87:  
movss (%rbx, %rdx, 4), %xmm0 # Get u[i]
mulss (%rax, %rdx, 4), %xmm0
adds %xmm0, %xmm1
addq $1, %rdx
cmpq %rcx, %rdx
jl .L87
# Multiply by v[i] #Addtosum
# Increment i
# Compare i to length # If <, keep looping)
*/

void inner(float *u, float *v, int length, float *dest) 
{ // This is the unoptimized function. 
	int i;
	float sum = 0.0f;
	for (i = 0; i < length; ++i)
		{
			sum += u[i] * v[i];
		}
	*dest = sum;
}

void inner2(float *u, float *v, int length, float *dest) 
{
	// four-way loop unrolling with four parallel accumulators (which is the contents of the for loop)
	int i; 			    // Intialize as much as possible before the for loop to get the most optimization.
	float sum1 = 0.0f;  // Setting them all to start at 0.
	float sum2 = 0.0f;
	float sum3 = 0.0f;
	float sum4 = 0.0f;
	int limit = length - 3; // Offsetting this to subtract 3 to reflect the 4 we are looking at at one time.

	for(i = 0; i < limit; i += 4) // pull out 4 at a time as just mentioned above.
	{
		// implementing four-way loop unrolling. See Lecture 5b Slide # 14/18
		sum1 = sum1 + u[i] * v[i];     // Creating a sum for each 4 we pull out.
		sum2 = sum2 + u[i+1] * v[i+1];
		sum3 = sum3 + u[i+2] * v[i+2];
		sum4 = sum4 + u[i+3] * v[i+3];
	}
	// This is cleaning up the array, Source: Lecture 5b, Slide 18.
	for(; i < length; i++)
	{
		sum1 = sum1 + (u[i] * v[i]);
	}
	*dest = sum1 + sum2 + sum3 + sum4; // destination is used to create a new array.
}

int main(int argc, char *argv[])
{
	clock_t start1; /* Initalizing the start and end times for both the inner 
	function (start1 / end1) and the optimized inner2 funciton (start2 / end2) */
	clock_t start2;
	clock_t end1;
	clock_t end2;
	float *u = malloc(sizeof(float) * N); // Mallocing
	float *v = malloc(sizeof(float) * N);
	float dest = 0;
	int num_func_calls = 1000; // This is the amount of how many times we run the functions. 

	for (int i=0; i<N; i++)
	{
		u[i]=i;  // This is filling the arrays as large as N which is a 
		v[i]=i;  // global variable defined at the top.
	}

	start1 = clock();
	for(int i=0; i<num_func_calls; i++) /* This is calling the function 1000 times (num_func_calls) so we can have a large, and 
		 						 more accurate amount of data. You can then divide the total_time by 1000 to get 
		 						 the average time it takes to run it once. */
								
	{
		inner(u, v, N, &dest);
	}
	end1 = clock();

	double total_time1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC; // This is calculating the time it took to execute the function.
	printf("This is the unoptimized run time for array a: %f\n", total_time1/num_func_calls); // Average time for one funciton call
	printf("This is the sum for inner: %f\n", dest);

	start2 = clock();
    for(int i=0; i<num_func_calls; i++)
	{
		inner2(u, v, N, &dest); // Optimized function. 
	}
	end2 = clock();
	double total_time2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
	printf("This is the unoptimized run time for array a: %f\n", total_time2/num_func_calls);
	printf("This is the sum for inner2: %f\n", dest);

	printf("This means this was %f times faster!\n", (total_time1-total_time2)*1000);

	return 0;
}