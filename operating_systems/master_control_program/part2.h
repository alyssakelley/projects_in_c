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
#include <signal.h>
#include <sys/wait.h>

int RUN_VAR = 0;

typedef struct
{
	char * current_command; // aka current arg
	char **args; // aka array of args
	pid_t PID;
	int arg_count;
} Process;

int count_the_args(char * current_line);
int count_file_lines(FILE * file_of_commands);
Process * malloc_process_array(int line_count, FILE * file_of_commands);
void parse_the_line(Process * command_line, char * line);
void create_array_of_commands(FILE * file_of_commands, Process * program);
void send_signal(int signal, Process my_process);
void sig_handler(int signum);
void free_process(Process * my_process);
void start_the_processes(Process * program, int line_count);
void stop_the_processes(Process * program, int line_count);
void continue_the_processes(Process * program, int line_count);
void execute_all_processes(Process * program, int line_count);
