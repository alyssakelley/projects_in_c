// Name: Alyssa Kelley
// Due: Jan. 23, 2018

// Call this program like so:
// gcc -std=c11 -g -o clock.exe clock.c testClock.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#ifndef CLOCK_H_
#define CLOCK_H_

typedef struct {
  // Initializing the ASCII clock hour, minute, and second.
	int hour;
	int minute;
	int second;
} ClockType;

// initClock function: Initialize the clock data structure
void initClock( ClockType *clock );

// printClock function: Print an ASCII clock showing cur_time as the time 
void printClock( const time_t cur_time, const ClockType *clock );

// Free up any dynamically allocated memory in the clock
void cleanClock( ClockType *clock);

#endif /* CLOCK_H_ */