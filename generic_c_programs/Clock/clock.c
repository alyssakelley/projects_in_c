// Name: Alyssa Kelley
// Due: Jan. 23, 2018

// Call this program like so:
// gcc -std=c11 -g -o clock.exe clock.c testClock.c

#include "clock.h"


void initClock( ClockType *clock )
{
	clock = malloc(sizeof(clock)); // We can malloc it like this since our struct is defined as 3 integers. 

	clock->hour = 0; // initializing it all to be 0 at first.
	clock->minute = 0;
	clock->second = 0;
}

void printClock(const time_t cur_time, const ClockType *clock )
{
	char entire_time[8][48][7]; // 8 rows, 48 columns (6 columns each num)

	int hour = clock -> hour;
	int minute = clock -> minute;
	int second = clock -> second;

	// printf("This is the current time --> %d:%d:%d\n", hour, minute, second);

	char * time_data_string = malloc(50 * sizeof(int));

	time_data_string[0] = hour/10; // adding the first digit of the hour to the string. (Ex. hour 12, this passes the 1)
	time_data_string[1] = hour%10; // adding the first digit of the hour to the string. (Ex. hour 12, this passes the 2)
	time_data_string[2] = ':';
	time_data_string[3] = minute/10;
	time_data_string[4] = minute%10;
	time_data_string[5] = ':';
	time_data_string[6] = second/10;
	time_data_string[7] = second%10;

	int i = 0; // i is our counter for the column in our for loop below. This will be incrementing by 6 in the loop.

	for(int digit = 0; digit < 8; digit++) // 01:34:67
	{
			if((time_data_string[digit] + '0') == '0') // Comparing the string 0 to 0 to see if that digit matches.
				// by adding it by the stirng 0 ('0'), that converts the interger to a string to be able to compare it.
			{
				//printf("0 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "#   # ");
				strcpy(entire_time[2][i], "#   # ");
				strcpy(entire_time[3][i], "#   # ");
				strcpy(entire_time[4][i], "#   # ");
				strcpy(entire_time[5][i], "#   # ");
				strcpy(entire_time[6][i], "#   # ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '1')
			{
				//printf("1 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "  #   ");
				strcpy(entire_time[1][i], "# #   ");
				strcpy(entire_time[2][i], "  #   ");
				strcpy(entire_time[3][i], "  #   ");
				strcpy(entire_time[4][i], "  #   ");
				strcpy(entire_time[5][i], "  #   ");
				strcpy(entire_time[6][i], "  #   ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '2')
			{
				//printf("2 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "#   # ");
				strcpy(entire_time[2][i], "    # ");
				strcpy(entire_time[3][i], "  #   ");
				strcpy(entire_time[4][i], "  #   ");
				strcpy(entire_time[5][i], "#     ");
				strcpy(entire_time[6][i], "#     ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '3')
			{
				//printf("3 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "    # ");
				strcpy(entire_time[2][i], "    # ");
				strcpy(entire_time[3][i], "# # # ");
				strcpy(entire_time[4][i], "    # ");
				strcpy(entire_time[5][i], "    # ");
				strcpy(entire_time[6][i], "    # ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '4')
			{
				//printf("4 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "#   # ");
				strcpy(entire_time[1][i], "#   # ");
				strcpy(entire_time[2][i], "#   # ");
				strcpy(entire_time[3][i], "#   # ");
				strcpy(entire_time[4][i], "# # # ");
				strcpy(entire_time[5][i], "    # ");
				strcpy(entire_time[6][i], "    # ");
				strcpy(entire_time[7][i], "    # ");
			}

			if((time_data_string[digit] + '0') == '5')
			{
				//printf("5 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "#     ");
				strcpy(entire_time[2][i], "#     ");
				strcpy(entire_time[3][i], "# # # ");
				strcpy(entire_time[4][i], "    # ");
				strcpy(entire_time[5][i], "    # ");
				strcpy(entire_time[6][i], "    # ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '6')
			{
				//printf("6 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "#     ");
				strcpy(entire_time[2][i], "#     ");
				strcpy(entire_time[3][i], "#     ");
				strcpy(entire_time[4][i], "# # # ");
				strcpy(entire_time[5][i], "#   # ");
				strcpy(entire_time[6][i], "#   # ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '7')
			{
				//printf("7 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "    # ");
				strcpy(entire_time[2][i], "    # ");
				strcpy(entire_time[3][i], "    # ");
				strcpy(entire_time[4][i], "    # ");
				strcpy(entire_time[5][i], "    # ");
				strcpy(entire_time[6][i], "    # ");
				strcpy(entire_time[7][i], "    # ");
			}

			if((time_data_string[digit] + '0') == '8')
			{
				//printf("8 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "#   # ");
				strcpy(entire_time[2][i], "#   # ");
				strcpy(entire_time[3][i], "# # # ");
				strcpy(entire_time[4][i], "#   # ");
				strcpy(entire_time[5][i], "#   # ");
				strcpy(entire_time[6][i], "#   # ");
				strcpy(entire_time[7][i], "# # # ");
			}

			if((time_data_string[digit] + '0') == '9')
			{
				//printf("9 - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "# # # ");
				strcpy(entire_time[1][i], "#   # ");
				strcpy(entire_time[2][i], "#   # ");
				strcpy(entire_time[3][i], "# # # ");
				strcpy(entire_time[4][i], "    # ");
				strcpy(entire_time[5][i], "    # ");
				strcpy(entire_time[6][i], "    # ");
				strcpy(entire_time[7][i], "    # ");
			}
 
			if(time_data_string[digit]  == ':') // The colon is already a char in the array so we do not have to add '0' to it.
			{
				//printf(": - I am putting %c in at rows 0-7 in col %d\n", time_data_string[digit] + '0', i);
				strcpy(entire_time[0][i], "      ");
				strcpy(entire_time[1][i], "      ");
				strcpy(entire_time[2][i], "  #   ");
				strcpy(entire_time[3][i], "      ");
				strcpy(entire_time[4][i], "      ");
				strcpy(entire_time[5][i], "  #   ");
				strcpy(entire_time[6][i], "      ");
				strcpy(entire_time[7][i], "      ");
			}

		i += 6; // incrementing the column position to change from 0 to 6 to 12 etc... depending on the digit position.
	}

 	// print the array:
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 43; j+=6)
		if( j == 42 )
		{
			printf(" %s \n", entire_time[i][j]); // goes through the entire array printing the string with a new line if it is the last column.
		}
		else
		{
			printf(" %s ", entire_time[i][j]); // goes through the entire array printing the string.
		}
	}

	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[0][0], entire_time[0][6], entire_time[0][12], entire_time[0][18], entire_time[0][24], entire_time[0][30], entire_time[0][36], entire_time[0][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[1][0], entire_time[1][6], entire_time[1][12], entire_time[1][18], entire_time[1][24], entire_time[1][30], entire_time[1][36], entire_time[1][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[2][0], entire_time[2][6], entire_time[2][12], entire_time[2][18], entire_time[2][24], entire_time[2][30], entire_time[2][36], entire_time[2][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[3][0], entire_time[3][6], entire_time[3][12], entire_time[3][18], entire_time[3][24], entire_time[3][30], entire_time[3][36], entire_time[3][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[4][0], entire_time[4][6], entire_time[4][12], entire_time[4][18], entire_time[4][24], entire_time[4][30], entire_time[4][36], entire_time[4][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[5][0], entire_time[5][6], entire_time[5][12], entire_time[5][18], entire_time[5][24], entire_time[5][30], entire_time[5][36], entire_time[5][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[6][0], entire_time[6][6], entire_time[6][12], entire_time[6][18], entire_time[6][24], entire_time[6][30], entire_time[6][36], entire_time[6][42]);
	// printf(" %s  %s  %s  %s  %s  %s  %s  %s  \n", entire_time[7][0], entire_time[7][6], entire_time[7][12], entire_time[7][18], entire_time[7][24], entire_time[7][30], entire_time[7][36], entire_time[7][42]);
}

void cleanClock( ClockType *clock)
{
	free(clock);
}

// the main is commented out due to being in the testClock.c file.

// int main()
// {
// 	ClockType *clock;
// 	initClock(clock);

// 	clock = malloc(sizeof(clock));

// 	time_t cur_time;
// 	time(&cur_time);

// 	struct tm * time_data;

// 	time_data = localtime(&cur_time);
// 	//printf("This is in main ====> %d:%d:%d\n", time_data->tm_hour, time_data->tm_min, time_data->tm_sec);

// 	clock -> hour = time_data -> tm_hour;
// 	clock -> minute = (time_data -> tm_min);
// 	clock -> second = (time_data -> tm_sec);
// 	//printf("%d is main clock hour.\n", clock->hour);

// 	printClock(cur_time, clock);
// 	cleanClock(clock);
// }

