/*

Authorship Statement

Name: Alyssa Kelley 
DUCK ID: alyssak
CIS 415 Project 1

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus (classmate), Quinn Milionis (classmate), Luke Peyralans (fellow CS student at UO),
and Zack Bower (fellow CS student at UO). 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct
{
	char * current_command; // aka current arg
	char **args; // aka array of args
	pid_t PID;
	int arg_count;
} Process;
