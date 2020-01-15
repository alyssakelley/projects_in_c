// Name: Alyssa Kelley
// Due: Jan. 23, 2018

// Call this program like so:
// gcc -std=c11 -o timer.exe timer.c testTimer.c

#include "clock.h"
#include "timer.h"

// Initialize the timer with the user-provided input
void initTimer(ClockType *clock, int minutes, int seconds)
{
	clock = malloc(sizeof(clock));
	clock->minute = minutes;
	clock->second = seconds;
}

int questionforTimer()
{
	int minutes;
	int seconds;
	int total_seconds;

	puts("How long should the timer run (MM:SS)?");
	scanf("%d:%d", &minutes, &seconds);
	//printf("This is in question. value: %d and mem: %p\n", minutes, &minutes);
	total_seconds = (minutes * 60) + seconds;
	return total_seconds; // returns this in total seconds so that the while loop is straight forward in the Timer function.
}


void printTimer(const ClockType *clock, int minute, int second) // minute and second is found in the Timer function
{
	char entire_time[8][30][7]; // allocating the memory for the array holding the ASCII representation.

	char * timer_data_string = malloc(50 * sizeof(int));

	timer_data_string[0] = minute/10; // adding the first digit of the minute to the string. (Ex. 12 minutes, this passes the 1)
	timer_data_string[1] = minute%10; // adding the second digit of the minute to the string. (Ex. 12 minutes, this passes the 2)
	timer_data_string[2] = ':';
	timer_data_string[3] = second/10;
	timer_data_string[4] = second%10;

	int i = 0;

	for(int digit = 0; digit < 5; digit++) // 01:34:67
	{
			if((timer_data_string[digit] + '0') == '0') // If the digit in the time is 0, then it forms a 0 in the array.
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

			if((timer_data_string[digit] + '0') == '1')
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

			if((timer_data_string[digit] + '0') == '2')
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

			if((timer_data_string[digit] + '0') == '3')
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

			if((timer_data_string[digit] + '0') == '4')
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

			if((timer_data_string[digit] + '0') == '5')
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

			if((timer_data_string[digit] + '0') == '6')
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

			if((timer_data_string[digit] + '0') == '7')
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

			if((timer_data_string[digit] + '0') == '8')
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

			if((timer_data_string[digit] + '0') == '9')
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
 
			if(timer_data_string[digit]  == ':') // Note: ASCII CODE FOR : IS 58
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
		for (int j = 0; j < 25; j+=6)
		if( j == 24 )
		{
			printf(" %s \n", entire_time[i][j]); // goes through the entire array printing the string.
		}
		else
		{
			printf(" %s ", entire_time[i][j]); // if it is the last col (j) in the array then prints the string with a new line at the end.
		}
	}
}

void Timer(ClockType *clock, int total_seconds)
{

	//printf("This is the recorded mins: %d, and the recorded secs: %d.\n", total_seconds/60, total_seconds%60);
	while(total_seconds != 0)
	{
		if(total_seconds > 0)
		{
			//printf("In while loop - first if statement.\n");
			//printf("%d:%d\n", total_seconds/60, total_seconds%60);
			int seconds = total_seconds%60;
			int minutes = total_seconds/60;

			printTimer(clock, minutes, seconds);
			printf("\n"); // printing a new line character to show the countdown better.

			total_seconds--; // decrease the seconds so that the countdown decreases.
			sleep(1); // delays the call by a second so we can see the countdown in seconds happening one by one.
		}
	}
}

// Run the timer -- print out the time each second --> this is printed using the printTimer function which the Timer function calls.
void runTimer()
{
	ClockType *clock;

	int minutes = 0; // initializing the values.
	int seconds = 0;
	int total_seconds = 0;

	total_seconds = questionforTimer(); // getting this number from user input in this function.
	minutes = total_seconds / 60;
	seconds = total_seconds % 60;

	initTimer(clock, minutes, seconds); // sets these values calculated in the clock struct from clock.h

	Timer(clock, total_seconds); // This starts the timer and calls on printTimer function.

	cleanTimer(clock); // frees the malloced clock.

}

void cleanTimer(ClockType  *clock)
{
<<<<<<< HEAD
	// free(clock);
=======
	//free(clock);
>>>>>>> 8bebe583e0cab8ea210549c984503756926579a3
}

// int main()
// {
// 	runTimer(); // most of the functions are called in runTimer and then the Timer function called the printTimer function.
// }