// Author: Alyssa Kelley
// Project 1, Part 2

#include <stdio.h>

/*write code for the following C function
that will return an unsigned int consisting of bytes
3 and 2 from x and bytes 1 and 0 from y:
*/

unsigned int combine(unsigned int x, unsigned int y)
{
unsigned int mask = (0xFFFF << 16);
unsigned int new_x = (x & mask);
unsigned int new_y = (y & ~mask);
unsigned int combined_word = (new_x | new_y);
// printf("0X%.8X\n", combined_word);
return combined_word;
}

int main(){
	unsigned new_word = combine(0x12345678, 0xABCDEF00);
	printf("0X%.8X\n", new_word);
	unsigned new_word_2 = combine(0xABCDEF00, 0x12345678);
	printf("0X%.8X\n", new_word_2);
	return 0;
}

