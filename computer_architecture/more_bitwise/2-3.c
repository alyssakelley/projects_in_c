// Author: Alyssa Kelley
// Assignment 2 Part 3

#include <stdio.h>

int ge(float x, float y) { /* This function will return 1 if x is greater than or equal to y, 
	and 0 if it is not. 1 = True and 0 = False. */
	unsigned int ux = *((unsigned int*) &x);  // convert x raw bits
	unsigned int uy = *((unsigned int*) &y);  // convert y raw bits
	unsigned int sx = ux >> 31;  // extraxt sign bit of ux. This will be 1 for neg and 0 for pos.
	unsigned int sy = uy >> 31;  // extract sign bit of uy.
	ux <<= 1;  // drop sign bit of ux. This will give you the value of x.
	uy <<= 1;  // drop sign bit of uy. This will give you the value of y.
	int less = ux <= uy;  /* This compares the actual number of ux and uy since 
	the sign has been stripped away from them and will return if ux is less than or 
	equal to uy */ 
	int great = ux >= uy; /* This compares the actual number of ux and uy since 
	the sign has been stripped away from them and will return if ux is greater than or 
	equal to uy */ 
	int sign_of = sx == sy;  /* This compares the signs of x and y only to see if the sign is
	the same or different */
	int bool_equal = ((less && !sign_of && (sx<sy)) || (ux == uy) || (great && sign_of && (sx==0)));  /* This finally gives 
	the value of True (1) or False (0) in regards to x being larger than y.
	(less && !sign_of) -> This is checking is num of x is less than y and if the signs are not equal to eachother. An example of this is: x = -3 y = 13 -> 0 
		and also x = 3 and y = -13 -> 1
	(ux == uy) -> This is checking to see if the actual number of x and y are the same. If this is the case then are returning 1. -0 and 0 are the same.
	(great && sign_of && (sx==0)) -> This is checking to see if x is greater than y and if the signs are the sign and if they are the same 
		then you need to check to see if the sign of x is equal to 0 or not, and if they are equal, then the signs are positive for x and y. 
		If you check this and they are not positive, then you would need to return False because that means that even though x is a bigger number
		without the sign, since both x and y are negative it is technically a bigger negative number which would make x less than y including their signs.
	*/
	return bool_equal;
}

int main(){
	int test_1 = ge(0.0f, 0.0f);	// expected outcome: 1
	printf("%d\n", test_1);
	int test_2 = ge(-0.0f, 0.0f);	// expected outcome: 1
	printf("%d\n", test_2);
	int test_3 = ge(-1.0f, 0.0f);	// expected outcome: 0
	printf("%d\n", test_3);
	int test_4 = ge(0.0f, 1.0f);	// expected outcome: 0
	printf("%d\n", test_4);
	int test_5 = ge(1.0f, 0.0f);	// expected outcome: 1
	printf("%d\n", test_5);
	int test_6 = ge(0.0f, -1.0f);	// expected outcome: 1
	printf("%d\n", test_6);
	int new_test = ge(-2.0, -1.0);   // expected outcome: 0
	printf("%d\n", new_test);
	int new_test2 = ge(-3.0, 13.0);   // expected outcome: 0. 
	printf("%d\n", new_test2);
	int new_test3 = ge(3.0, -13.0);   // expected outcome: 1. 
	printf("%d\n", new_test3);
	return 0;
}

/* The main function provides the test cases to see if the numbers are being 
compared to each other properly or not, the expected outcome is commented 
next to the function call. I choose to have my main test the variables and
print them individually instead of just one print statement in case I want to 
change my test cases around. */ 
