#include <stdio.h>

/* write code for the following C function that will return
1 if x has at least one bit with a value of 1 at an odd index, 0 otherwise
*/

int oddBit(unsigned int x)
{
unsigned isolated_odd_bits = x & 0xAAAAAAAA; //10101010101010101010101010101010
unsigned answer = !!(isolated_odd_bits);
// printf("%d\n", answer);
return answer;
}

int main()
{
unsigned test_1 = oddBit(0x2);
printf("%d\n", test_1);
unsigned test_2 = oddBit(0x3);
printf("%d\n", test_2);
unsigned test_3 = oddBit(0x4);
printf("%d\n", test_3);
unsigned test_4 = oddBit(0xFFFFFFFF);
printf("%d\n", test_4);
unsigned test_5 = oddBit(0xAAAAAAAA);
printf("%d\n", test_5);
unsigned test_6 = oddBit(0x55555555);
printf("%d\n", test_6);

return 0;
}
