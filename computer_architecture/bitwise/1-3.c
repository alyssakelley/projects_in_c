//Author: Alyssa Kelley
//Project 1, Part 3

#include <stdio.h>

/* write code for the following C function that will return an
unsigned int such that byte i of x is replaced by byte b
*/

unsigned int replace(unsigned int x, int i, unsigned char b)
{
unsigned mask = ((unsigned) 0xFF) << (i << 3);
unsigned insertion = ((unsigned)b) << (i << 3);
unsigned masked_word = x & ~mask;

unsigned new_word = masked_word | insertion;
// printf("0X%.8X\n", new_word);
return new_word;
}

int main()
{
unsigned test_1 = replace(0x12345678, 2, 0xAB);
printf("0X%.8X\n", test_1);
unsigned test_2 = replace(0x12345678, 0, 0xAB);
printf("0X%.8X\n", test_2);
return 0;
}
