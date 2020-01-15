// Author: Alyssa Kelley
// Assignment 4 Part 1

#include <stdio.h>
/*

Slide: 13

e in front = 32 bit, r in front = 64 bit. 

Note: %rdi = a
	  %rsi = b
	  %rdx = mask

In terms of: a, b, result, and mask

loop:
	movq %rsi, %rcx. --> b (rsi) is being put into rcx.
	movl $1, %edx.   --> This is setting the mask to 1.
	movl $0, %eax    --> This is setting the mask != 0 and being held in eax.
.L2:
	testq %rdx, %rdx --> This is the part for the mask (mask != ) since it is testing if they are not the same.
	je .L4           --> jump to L4 if = 
	movq %rdx, %r8   --> moving the max into r8. For the mask since you know the mask is being moved.
	andq %rdi, %r8   --> (a & mask)
	orq %r8,%rax     --> result |= r8 which was (a & mask) 
	salq %cl, %rdx   --> This left shifts the mask over by cl (last 8 bits)
	jmp .L2			 --> jump to L2
.L4:
	ret              --> return result %rax

*/
long loop(long a, long b) {
	long result = 0; 	// movl $0, %eax
	for (long mask = 1 ; mask != 0 ; mask <<= b ) {		// movl $1, %edx, testq %rdx, %rdx, salq %cl, %rdx
        result |= (a & mask);	// orq %r8,%rax
    }
    return result; 	// ret
}
int main(){
	int test_1 = loop(1, 5);  // This should return 1.
	printf("%d\n", test_1);

	int test_2 = loop(2, 4);  // This should return 0.
	printf("%d\n", test_2);

	int test_3 = loop(3, 3);  // This should return 1.
	printf("%d\n", test_3);

	int test_4 = loop(4, 2);  // This should return 4.
	printf("%d\n", test_4);

	int test_5 = loop(5, 1);  // This should return 5.
	printf("%d\n", test_5);

	return 0;
}