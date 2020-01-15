#include <stdio.h>

int main()
{
	int prime_num, divisor; /* prime_num = the number we are checking to see if prime or not, 
		  		 * and divisor = the number it is trying to be divisable by.
		   		 */

	for(prime_num=5; prime_num<=100; prime_num++)   
	{

		/* this prime_num++ means that it is incrementing a in steps of 1
		so it will be counting up an integer each time to go up to 100
		*/

		for(divisor=2; divisor<=prime_num; divisor++)
		{
			/* this will have the divisor start at 2 and increment up and it 
			 *divides against the prime_num to see if there is a remainder or not 
			 */

			if(prime_num%divisor==0) /* if this is true then that means
						  * they are divisable and prime_num variable is NOT a prime number
						  */
			{	
				break; /* Gets us out of the inner for loop and not the outer one */
			}
		}

		if(prime_num==divisor)
		{ 
			/*this is after that division for loop so if all of those went through 
			 *so this checks that if the two numbers are equal, and the remainder 
			 *was not equal to 0, then they are divisable and prime_num is a prime number.
			 */	

			printf("%d is a prime number\n", prime_num);
			
		}
	}
	return 0;
}
