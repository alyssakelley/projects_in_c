// Author: Alyssa Kelley
// Assignment 6 - Part 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 800

typedef int array_t[N][N];

int dim() 
{
    return N;
}
void old_f(array_t a, int x, int y) 
{
	for (int i = 0; i < dim(); ++i) 
	{
		for (int j = 0; j < dim(); ++j) 
			{ 
				a[i][j] = i * x * y + j; // Change this part!
			}
	}
}
/* 
Task: Rewrite the above procedure f to minimize unnecessary function calls and multiplications. Note that you are not required to replace the a[i][j] lookups with pointer arithmetic; focus on
the i * x * y + j equation with respect to removing unnecessary multiplications. Also write a main() function to test your procedure. Name your source file 6-1.c.
*/

void _f(array_t a, int x, int y)
{
	int *beginning = &a[0][0]; // starting the array at the beginning location
	int len = dim(); /* Creating the length as a variable so it does not have to waste that time 
						continuing to reference another function (dim()) */
	int mul_xy = x*y; // Pulling this multiplication out of loop compisition to further optimize
	int mul_xyi; 
	int i = 0; // initalizing here so it is less costly in the for loops
	int j = 0;

	for (i = 0; i < len; i++)
	{
		mul_xyi = mul_xy * i; // Pulling the multiplication outside of the inner for loop.
		for (j = 0; j < len; j++)
		{
			//*pointer = mul_xyi + j; // a[i][j] = i*x*y + j 
			a[i][j] = mul_xyi + j; // Only adding here because it is less costly.
		}
	}
}

void print_array(array_t a) // This is a function just to print the array in an ideal format.
{
	printf("{ ");
	for (int i = 0; i < N; ++i)
	{
		printf("{");

		for (int j = 0; j < N; ++j)
		{
			if (j < N-1)  // Only printing a comma after the number if it is not the last one in the sub arrays.
			{
				printf("%i, ", a[i][j]);
			}
			else
			{
				printf("%i ", a[i][j]);	
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
	clock_t start, end;
	double total_time;
	array_t array_1; 
	array_t array_2;

	int x = 2;
    int y = 2;

	start = clock();
	for(int i=0; i<1000; i++)
	{
		old_f(array_1, x, y);
	}
	//old_f(a, x, y);
	end = clock();
	total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("This is the unoptimized run time for array a: %f\n", total_time); // you can divde here by 1000 to get an average
    //print_array(a);

	// Optimized time for array a
	start = clock();
	//_f(a, x, y);
	for(int i=0; i<1000; i++)
	{
		_f(array_1, x, y);
	}
	end = clock();
    //print_array(a);
    total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("This is the optimized run time for array a: %f\n", total_time);
	//print_array(a); 
    
	start = clock();
	// old_f(b, x, y);
	for(int i=0; i<1000; i++)
	{
		old_f(array_2, x, y);
	}
	end = clock();
	total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	//print_array(b);
    printf("This is the unoptimized run time for array b: %f\n", total_time);

	start = clock();
    //_f(b, x, y);
    for(int i=0; i<1000; i++)
	{
		_f(array_2, x, y);
	}
    end = clock();
	total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    //print_array(b);   
    printf("This is the optimaized run time: %f\n", total_time);

    return 0;
}