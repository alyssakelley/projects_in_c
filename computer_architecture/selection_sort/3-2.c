// Author: Alyssa Kelley
// Project: Assignment 3 Question 2

#include <stdio.h>

long decode (long a, long b, long c){ /* These decode values also read as: (rdi, rsi, rdx) */
	int x; /* This is a variable that we create later on that is a new register. */
	b = b - c; 
	/* This is for the instruction: subq %rdx, %rsi, which translated so the following meaning:
	Src, Dest -- Dest = Dest - Src */
	a = a * b; 
	/* imulq %rsi, %rdi 
	Dest = Dest * Src */
	b = b << 63; 
	/* salq $64, %rsi -- This is 63 since we are working with 64 bits 
	Dest = Dest << Src */
	b = b >> 63; 
	/* sarq $63, %rsi
	Dest = Dest >> Src */
	x = a; 
	/* movq %rsi, %rax
	x is a new register we have for rax */
	x = x ^ b; 
	/* xorq %rsi, %rax 
	Dest = Dest ^ Src */
	return x;
}

int main(){

	int test_1 = decode(1, 2, 4);
	printf("%d\n", test_1); 	/* Should return -2 */
	int test_2 = decode(3, 5, 7);
	printf("%d\n", test_2); 	/* Should return -6 */
	int test_3 = decode(10, 20, 30);
	printf("%d\n", test_3); 	/* Should return -100 */
	return 0;
	}