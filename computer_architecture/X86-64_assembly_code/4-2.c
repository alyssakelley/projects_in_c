// Author: Alyssa Kelley
// Assignment 4 Part 2

#include <stdio.h>

int DW_sum(int from, int to) {
    int result = 0;
    do {
        result += from;
        ++from;
    } while (from <= to);
    return result;
}

long sum(long from, long to) {
     long result = 0;
// Ensure that argument *from* is in %rdi,
// argument *to* is in %rsi, *result* is in %rax - do not modify. 
     __asm__ ("movq %0, %%rdi # from in rdi;" :: "r" ( from )); 
     __asm__ ("movq %0, %%rsi # to in rsi;" :: "r" ( to ));
     __asm__ ("movq %0, %%rax # result in rax;" :: "r" ( result ));
     // Your x86-64 code goes below - comment each instruction...
     /* Please see notes below main for my conversion from the do-while sum -> goto -> asm. This 
        shows my thought process. */
     __asm__ (".Loop:");  // Creating our loop label. 
     __asm__ ("addq %rdi, %rax");  // This is adding the result (rax) with (rdi)/from to update result (rax)
     __asm__ ("addq $1, %rdi");  // Incrementing (rdi)/from by 1.
     __asm__ ("cmpq %rsi, %rdi");  // Comparing (rsi)/to to (rdi)/from.
     __asm__ ("jle .Loop");  // if from is not less than/= to (less than =jle), then jump to loop
    // ("movq %rsi, %rax # For example, this sets result = to;");
	// Ensure that *result* is in %rax for return - do not modify.
    __asm__ ("movq %%rax, %0 #result in rax;" : "=r" ( result )); 
     return result;
}

int main()
{
	int test_1 = sum(1,6);  // Result should be 21
	printf("%d compared to: %d\n", test_1, DW_sum(1,6));

	int test_2 = sum(3, 5); // Result should be 12
	printf("%d compared to: %d\n", test_2, DW_sum(3,5));

	return 0;
}
/*

* Notes from asm:

	RESULT = RAX
	FROM = RDI
	TO = RSI (you will compare using this to see if done.)

	recall: addq (Src, Dest). -> Dest = Dest + Src
	
* Converting to a goto version of the loop:

	loop:
    result += from & 0x1;	adding results and then stores new result there
    from >>= 1;				shifting/incrementing (add 1)
    if(from) goto loop;		using an if to see if needing to loop again and then goes to the loop
    return results			return result 

* Converting to semi-assembly language:

	LOOP:
	add (rdi, rax) this is adding the result (rax) with from (rdi) to update result (rax)
	add (1, rdi) this increments (rdi) the from number by 1 
	compare (rsi, rdi)
	jump back to LOOP
		
*/