// Name: Alyssa Kelley
// Due: Jan. 23, 2018

// Call this program like so:
// gcc -std=c11 -o timer.exe timer.c testTimer.c

// Initialize the timer with the user-provided input

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "clock.h"
#include <unistd.h>

void initTimer(ClockType *clock, int minutes, int seconds);

// Run the timer -- print out the time each second
void runTimer();

// Clean up memory (as needed)
void cleanTimer(ClockType  *clock);