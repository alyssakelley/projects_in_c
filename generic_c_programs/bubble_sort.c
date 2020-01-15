#include <stdio.h>

int main()
{
    int A[100] = { 252, 657, 268, 402, 950, 66, 391, 285, 133, 577, 649, 166, 987, 314, 954, 214, 920, 230, 904, 801, 40, 552, 369, 682, 202, 712, 395, 517, 755, 603, 134, 385, 428, 941, 443, 477, 95, 647, 687, 737, 673, 19, 325, 697, 577, 181, 45, 964, 267, 600, 858, 145, 781, 760, 949, 508, 673, 717, 446, 634, 635, 679, 466, 474, 916, 855, 216, 899, 804, 159, 237, 625, 963, 388, 437, 682, 821, 325, 805, 876, 968, 414, 190, 434, 902, 794, 752, 729, 77, 243, 705, 953, 765, 637, 765, 158, 166, 599, 70, 927 };
	
	/* Using bubble sort. This is going to go through each number in the array to compare the two, and
	 * possibly change positions so the int in the array that is larger goes behind
	 * the other integer, this is putting them in lesser than to greater than order */
	
	int pos_i;
		while (1){
			int is_it_sorted = 0; 	/* Having this remain 0 would indicate that it is sorted. */
			for (pos_i = 0; pos_i < 100-1; pos_i ++){
				if (A[pos_i] > A[pos_i + 1]){
					int temporary_holder = A[pos_i];
					A[pos_i] = A[pos_i + 1];
					A[pos_i + 1] = temporary_holder;
					is_it_sorted = 1; /* since we has to swap numbers, it is not sorted yet so this is 1 */
				}
			}
			
			if (is_it_sorted == 0){ 	/* 0 = it is sorted so we are done! */
				break;
			}
		}
		
	for (int i = 0; i < 100; ++i){ 	/* This is going through the array and printing a tab before the integer in the array
		and if the array can be divided by 10 that means we will need to start a new line so there are only 10 integers
		in each row that is printed. */
		printf("\t%i", A[i]);
		if ((i+1)%10 == 0){
			printf("\n");
		}
	}	
	return 0;
}