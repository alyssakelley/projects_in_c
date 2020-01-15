#include <stdio.h>

int main()
{
	int A[21] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 
		377, 610, 987, 1597, 2584, 4181, 6765};
	int *B[3];
	B[0] = A;  //location
	B[1] = A+7;  // location
	B[2] = A+14;  //location

	printf("%p is pointing to the location of B[0] and this lines up with pointer for %p for A\n", B[0], A);
	printf("%p is pointing to the location of B[1] and this lines up with value %d for A[7]\n", B[1], A[7]);

	printf("%p is the address for A[7]\n", &A[7]);
	printf("%p is pointing to the location of B[2] and this lines up with value %d for A[14]\n", B[2], A[14]);
	printf("%p is the address for A[14]\n", &A[14]);

	printf("%d is the value for A[0]\n", A[0]);
	printf("%p is the location for A[0]\n", &A[0]);

	printf("%d is the value\n", B[1][3]);

	printf("%p is the value for A\n", A);
	printf("%d is the value for *A\n", *A);
	printf("%d is the value for *(A+4)\n", *(A+4));
	printf("%p is the location for (A+6), and this is the value %d\n", (A+6), *(A+6));

	printf("%d is the value of B[2][0] - B[1][6]\n", (B[2][0] - B[1][6]));

	printf("%d is the value of B[2][0] - B[1][7]\n", (B[2][0] - B[1][7]));

	printf("%ld is the value of (A+6) - (A+3)\n", ((A+6) - (A+3)));
	printf("%d this is the value of (A+6) and %d this is the value of (A+3)\n", *(A+6), *(A+3));

	printf("\n Row 2, Q 1 problem:\n");
	printf("%p is the value for (A+6)\n", (A+6));
	printf("%p is the value for (A+3)\n", (A+3));
	printf("%ld is the location for (A+6)-(A+3).\n", ((A+6)-(A+3)));

	printf("\nRow 2, Q 2 problem\n");
	printf("%d is the value for *(A+6)\n", *(A+6));
	printf("%d is the value for *(A+2)\n", *(A+2));
	printf("%d is the value for *(A+6)-*(A+2).\n", (*(A+6)-*(A+2)));

	printf("\nRow 2, Q 3 problem:\n");
	printf("%d is the value for A[5]\n", A[5]);
	printf("%d is the value for *(A+4)\n", *(A+4));
	printf("%d is the value for A[5]-*(A+4).\n", (A[5]-*(A+4)));

	printf("\nRow 2, Q 4 problem:\n");
	printf("%p is the value for (A+9)\n", (A+9));
	printf("%p is the value for B[0]\n", B[0]);
	printf("%ld is the value for (A+9)-B[0].\n", ((A+9)-B[0]));

	printf("\nRow 2 - Q 5: %p this is the location for B[0]\n", B[0]);

	printf("\nRow 2, Q 7:\n");
	printf("%p is the value for &(B[2][3])\n", (&(B[2][3])));
	printf("%p is the value for &(A[15]).\n", (&(A[15])));
	printf("%ld is the value for (&(B[2][3]) - &(A[15]))\n", (&(B[2][3]) - &(A[15])));

	printf("\nRow 2, Q 8:\n");
	printf("%d is the value for A[6]])\n", A[6]);
	printf("%p is the location for &A[6]])\n", &A[6]);
	printf("%d is the value for *(&A[6]]))\n", *(&A[6]));

	printf("\nCurrent problem:\n");
	printf("%d is the memory location for B[2][2]])\n", B[2][2]);
	printf("%d is the location for A[15]])\n", A[15]);
	printf("%d is the value for B[2][2] - A[15])\n", B[2][2] - A[15]);









	printf("***************************\n");
	printf("%p is the address of B[2]\n", B[2]);
	printf("%d is the value of B[2] dereferenced by *\n", *B[2]);
}






