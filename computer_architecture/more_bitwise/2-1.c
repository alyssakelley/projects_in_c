#include <stdio.h>

int mask(int n){
	unsigned int mask = 0xFFFFFFFE; 	/* mask = 1111 1111 1111 1111 1111 1111 1111 1110. This last part has a 0 in it
	to act as a marker when it is shifted later on. */
	unsigned int mask_sh_left = mask << (31 - n); /* This is shifted so that all of the 1's will be on the 
	most significant side (left) which is allowing for the specific part of the n that is wishing to be 
	kept the same */
	unsigned int mask_sh_right = mask_sh_left >> (32 - n); /* After you have the specific section that you want 
	from n, it is then shifted over to the very end so that that specific section is all the way to the right 
	(the least significant section) and the rest is only 0's. */
	return mask_sh_right;
}

int main(){
	int test_1 = mask(0);
	printf("0X%X\n", test_1);
	int test_2 = mask(1);
	printf("0X%X\n", test_2);
	int test_3 = mask(2);
	printf("0X%X\n", test_3);
	int test_4 = mask(3);
	printf("0X%X\n", test_4);
	int test_5 = mask(5);
	printf("0X%X\n", test_5);
	int test_6 = mask(8);
	printf("0X%X\n", test_6);
	int test_7 = mask(16);
	printf("0X%X\n", test_7);
	int test_8 = mask(31);
	printf("0X%X\n", test_8);
}

/* You can then see that in my main function I called the mask function with the different 
test cases that was in the project spec and then printed them in the specific hex format 
but not the entire 08 format.
*/ 