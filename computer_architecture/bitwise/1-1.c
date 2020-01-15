// Author: Alyssa Kelley
// Project 1, Part 1

#include <stdio.h>

void printBytes(unsigned char *start, int len) {
    for (int i = 0; i < len; ++i) {
        printf(" %.2x", start[i]);
}
    printf("\n");
}
void printInt(int x) {
    printBytes((unsigned char *) &x, sizeof(int)); /* This &x is pointing to a memory of x location 
						    * and the size is how far we are going to go look
						    * in memory
						    */
}

void printFloat(float x){  // a float is 32 bits long.
    printBytes((unsigned char *) &x, sizeof(float));
}

void printShort(short x){ // a short is 16 bits long.
    printBytes((unsigned char *) &x, sizeof(short));
}

void printLong(long x){ // a long is 64 bits long.
    printBytes((unsigned char *) &x, sizeof(long));
}

void printDouble(double x){ // a double is 64 bits long.
    printBytes((unsigned char *) &x, sizeof(double));
}
 
void test_show_Bytes(int val){
   int ival = val;
   float fval = (float) ival; 
   int pval = ival;
   short sval = (short)(ival);
   long lval = (long)(fval);
   double dval = (double)(pval);
   printInt(ival);
   printFloat(fval);
   printShort(sval);
   printLong(lval);
   printDouble(dval);
}

int main(){
   int val = 0x1234ABCD;
   test_show_Bytes(val);
}

/* Comments:
 * With the example of 0x1234ABCD, you get:
 * cd ab 34 12 - This is Little Endian format where the least significant bits go to the lower
 * memory slot at the beginning.
 * 5e a5 91 4d - for float rep it is different due to the shifting of the bits so it is getting
 * rearranged.
 * cd ab - Short rep it is representing the 2 most significant bytes creating a 
 * total of 16 bits with ab still being the most significant.
 * c0 ab 34 12 00 00 00 00 - Long adds an additional 4 bytes to the word since longs are 64 bits. 
 * which you can see is all 0's in this representation.
 * 00 00 00 cd ab 34 b2 41 - the double adds in extra 0 bits and then that
 * shifts the rest of the value.
 */
