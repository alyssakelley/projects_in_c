#include <stdio.h>

unsigned int extract(unsigned int x, int i){ /* This function will return byte i of x signed
	extended to 32 bits */
	unsigned int multipled_i = (i << 3); /* By shifting i by 3 that multiple the decimal representation of byte i
	by 3 so that it will give you the decimal representation of how many bits need to be shifted over. It basically
	changed the decimal byte location into how many bits shifted over. */
	int shifted_by_i = x << (24 - multipled_i); /* You shift x over by 24-byte so that the byte you are wanting
	to return and signed extend at the left most position (most sig) so that it will be byte00000 */
	int sign_extended_x = shifted_by_i >> 24; /* After the byte is all the way on the left, this will 
	allow it to be sign extended since now the most signification bit is the sign bit for that byte. It is shifted back 
	over by 24 since that is what we just subtracted the bit from so it balances out to maintane the output to still
	be a total of 32 bits */
	return sign_extended_x;
}

int main(){
	int test_1 = extract(0x12345678, 0);	// expected output: 0x00000078
	printf("0X%.8X\n", test_1);
	int test_2 = extract(0xABCDEF00, 2);	// expected output: 0xFFFFFFCD
	printf("0X%.8X\n", test_2);
	return 0;
}

/* You see the two test cases from the spec in the main function and the sign extended byte is 
represented in full hexidecimal notation. */