#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Author: Alyssa Kelley
// Assigment 7 - Part 2

/* Notes for 7-2:

0 x 1  2  3  4  5  6   7    8
  |________________|  |_|  |_|
         tag          set offset
*/

unsigned int getOffset(unsigned int address)
{
  /* This is going to return the right most bit.
     Example: 0x12345678 -> Offset = 8
  */
	unsigned int bitmask = 0xF; // Using the mask 1111 to get the last 4 bits to indicate the last number.
  unsigned int offset = address & bitmask; // & these together will have 0 everyhting else except for the last 4 bits.
	return offset; // Returning the right most 4 bits.
}

unsigned int getTag(unsigned int address)
{
  /* This is going to return the left most 4*6 bits.
     Example: 0x12345678 -> Tag = 123456
  */
	unsigned int bitmask = 0xFFFFFF00; /* Using the bit mask 1111 1111 1111 1111 1111 1111 0000 0000 
  and the reason for this is to get only the first 6 integers and then the last two should be dropped. */
  unsigned int set = (address & bitmask); 
	return set >> 8; // Shifting off 8 so get rid of the last 2 0'd out integers.
}

unsigned int getSet(unsigned int address)
{
  unsigned int bitmask = 0x000000F0; /* Using the mask 0000 0000 0000 0000 0000 0000 1111 0000 and 
  this will get the second to last integer in the hex number. */
  unsigned int tag = address & bitmask;
  return tag >> 4; // This shifts off the last integer before the second to last 
}

int main() 
{
  unsigned int address1 = 0x12345678;
  unsigned int address2 = 0x87654321;

  printf("0X%.8X: offest: %d - tag: %X - set: %X\n", address1, getOffset(address1), getTag(address1), getSet(address1));
  printf("0X%.8X: offest: %d - tag: %X - set: %X\n", address2, getOffset(address2), getTag(address2), getSet(address2));

	return 0;
}
