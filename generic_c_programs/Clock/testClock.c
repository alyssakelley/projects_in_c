// Name: Alyssa Kelley
// Due: Jan. 23, 2018

// Call this program like so:
// gcc -std=c11 -g -o clock.exe clock.c testClock.c

#include "clock.h"

int main()
{
	ClockType *clock;
	initClock(clock);

	clock = malloc(sizeof(clock));

	time_t cur_time;
	time(&cur_time);

	struct tm * time_data;

	time_data = localtime(&cur_time); // provides us the current time.
	//printf("This is in main ====> %d:%d:%d\n", time_data->tm_hour, time_data->tm_min, time_data->tm_sec);

	clock -> hour = time_data -> tm_hour; // updating the struct values to use int he printClock function.
	clock -> minute = (time_data -> tm_min);
	clock -> second = (time_data -> tm_sec);
	//printf("%d is main clock hour.\n", clock->hour);

	printClock(cur_time, clock);
	cleanClock(clock);
}