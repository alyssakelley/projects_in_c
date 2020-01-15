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
#include <sys/time.h>
#include <sys/wait.h>

int RUN_VAR = 0;
int ACTIVE = 0;
int EXIT_PROG = 0;
int ALL_EXITED = 0;
int NUM_PROCS = 0;

enum State { NotStarted = 0, Running = 1, Paused = 2, Exited = 3 };

typedef struct
{
	char * current_command; // aka current arg
	char **args; // aka array of args
	pid_t PID;
	int arg_count;
	enum State status_state; // sigalarm owns state
	int has_exited; // sigchild owns has_exited
} Process;

Process * program;
int CurrentProcess = 0;

int count_the_args(char * current_line);
int count_file_lines(FILE * file_of_commands);
Process * malloc_process_array(int line_count, FILE * file_of_commands);
void parse_the_line(Process * command_line, char * line);
void create_array_of_commands(FILE * file_of_commands, Process * program);
void send_signal(int signal, Process my_process);
void sig_handler(int signum);
void sig_handler_2(int signum);
void kill_process(int PID);
int check_if_currentproc_exited(pid_t PID);
void free_process(Process * my_process);
void schedule_processes(sigset_t signal_set, int sig);
void schedule_entire_program(Process * program, sigset_t signal_set_ALRM, int sig);
void start_the_processes(Process * program);
void stop_the_processes(Process * program);
void exit_the_processes(Process * program);
void print_the_info(Process * program);
void execute_all_processes(Process * program);
