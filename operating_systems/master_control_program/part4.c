/*

Authorship Statement

Name: Alyssa Kelley 
DUCK ID: alyssak
CIS 415 Project 1

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus (classmate), Quinn Milionis (classmate), Luke Peyralans (fellow CS student at UO),
and Zack Bower (fellow CS student at UO). 

*/

#include "part4.h"

int count_the_args(char * current_line)
{
	int arg_count = 0;

	int i;

	for(i = 0; current_line[i] != '\0'; i++)
	{
		if(current_line[i] == ' ')
		{
			arg_count++;
		}
	}

	return arg_count + 1; // add one bc first arg (cmd) not here
}

int count_file_lines(FILE * file_of_commands)
{
	/*
	This function should go through the file, and create
	a line counter (which is the number of commands
	for the program) and returns this int.
	*/
	int number_of_commands = 0; // line_count for file

	char line[1024];

	while(fgets(line, sizeof(line), file_of_commands) != NULL)
	{
		number_of_commands++; // incrementing the line count until end of file
	}

	return number_of_commands;
}


Process * malloc_process_array(int line_count, FILE * file_of_commands)
{
	/*
	This will malloc for the cluster of processes which I am
	referring to as the program.
	*/

	Process * my_process = malloc(sizeof(Process) * line_count);

	int i;
	char line[1024];
	int arg_count = 0;

	for(i = 0; i< line_count; ++i)
	{
		fgets(line, sizeof(line), file_of_commands);
		arg_count = count_the_args(line);

		my_process[i].current_command = NULL;
		my_process[i].args =  malloc(sizeof(char *) * (arg_count + 1));
		my_process[i].args[arg_count] = NULL;
		my_process[i].PID = 0; // 0 = failure
		my_process[i].arg_count = arg_count;
		my_process[i].has_exited = 0;
		my_process[i].usertime = 0;
		my_process[i].systemtime = 0;
		my_process[i].status_state = NotStarted; // initializing

	}

	return my_process; // this is pointing to a bunch of processes (all the lines from input)
}

void parse_the_line(Process * command_line, char * line)
{
	/*
	This will pull the command out from the line, and then it will
	create the arguments for that command. The Process struct 
	will keep track of all this. Each line has their own Process
	struct.
	*/
	char * copy_of_line;
	char * command_name;
	char * current_arg;

	copy_of_line = strdup(line);

	command_name = strtok(copy_of_line, " "); // this gets the first word

	command_line->current_command = command_name;
	command_line->args[0] = command_name;

	int counter = 1;
	current_arg = strtok(NULL, " ");
	while(current_arg != NULL)
	{
		command_line -> args [counter] = current_arg;
		current_arg = strtok(NULL, " ");
		counter++;
	}
}


void create_array_of_commands(FILE * file_of_commands, Process * program)
{
	/* This function is in charge of creating all of the processes from the program.
	So, if the program test.txt has 3 commands in it, then this function will go 
	through test.txt and read it through then call parse_the_line which will 
	save the information for the process in it's own struct, and then the 
	program with be a cluster of those processes. All in all, create 3 process structs
	and the process program struct.
	*/

	char line[1024];

	int num_of_commands = 0;

	while(fgets(line, sizeof(line), file_of_commands) != NULL) // looping through the lines
	{
		line[strlen(line) - 1] = '\0'; // getting rid of null issue

		parse_the_line(&program[num_of_commands], line); // parsing the function
		num_of_commands++; // this will go to the next process in the cluser of processes (program)
	}
}

void send_signal(int signal, Process my_process)
{
	/* 
	This is a function specifically to send signals to a process.
	*/
	kill(my_process.PID, signal);
}

void sig_handler(int signum)
{
	/* Specific function to create the RUN_VAR 
	which is used in main for SIGUSR1 aka running. 
	*/
	if (signum == SIGUSR1)
	{
		RUN_VAR = 1;
	}
}


int check_if_currentproc_exited(pid_t PID)
{
	/* 
	This function is like Roscoe's sigchildhandler function which goes
	through to check if the status of the process is exited or not, 
	and if it has exited it return 1, and if it has not exited, 
	it returns 0, and this function is used in exit_the_processes
	to make sure that all the processes that have been exited get their 
	status changed to Exited.
	*/
	int has_exited = 0;
	int status = 0;
	if(waitpid(PID, &status, WNOHANG) > 0)
	{
		if(WIFEXITED(status))
		{
			has_exited = 1;
		}
		else
		{
			has_exited = 0;
		}
	}
	return has_exited;
}


void schedule_processes(sigset_t signal_set, int sig)
{
	/* This function is purely calling the sigwait function. */
	sigwait(&signal_set, &sig);
}

void schedule_entire_program(Process * program, sigset_t signal_set_ALRM, int sig)
{
	/* 

	This function is simular to Roscoe's sigalarmhandler function. 
	It starts off by blocking the alarm so it does not finish prematurely 
	and then I set the quantum value to use in the sigwait which is called 
	in the schedule_process function. 

	Step 1) Then I check to see if the current process is running or not, and if it is
	Running, then I check to see if it has exited meaning it is finished and 
	decrement the number of active processes and I change the state to Excited.

	If it is running but has not exited, then I check to see if it is the 
	last process left to go and if that is the case, I do not want to change the
	status of it, because I want that process to just complete. However, if it
	is not the last active process, and it is not exited, then I send that process
	SIGSTOP and change the status to Paused and.

	Note: Step 1 will not do anything during the first iteration because all of the process
	are stopped (Paused) or terminated (Exited) before this function is ever called in main.

	Step 2) I find the next process to start up by looping through the entire processes
	again starting at the CurrentProcess global variable. Note: It is important to add
	the CurrentProcess by the number of processes we have (NUM_PROCS) because we want
	this to be able to wrap around since it is a Round Robin scheduler and needs to continue 
	the cycle. For this same reason, I incorporated mod division in order to have this wrap
	around method anytime I use the iterator i. In this loop, if the process is Paused, 
	then I found the next process I need for my loop and I break out of it. Otherwise,
	the loop will finish and we will check if i is equal to that value of CurrentProcess + NUM_PROC
	and if it is, then the loop finished and I didn't find any process that was Paused so
	the program is finished.

	Step 3) Now that the next processs has been determined, or the loop has been exited, 
	I used that nextProc value and sent the SIGCONT singal to that process and change it's
	state to Running. Then I made sure to update the CurrentProcess global variable for the
	next iteration through.

	Note: It is important to reset the ualarm at the end. This is done at both 
	steps of when it is the very last active process running, and when the next process
	is sent the SIGCONT and starts running.

	*/
	sigprocmask(SIG_BLOCK, &signal_set_ALRM, NULL); // blocking sigalarm handler

	int quantum = 10000;
	ualarm(quantum, 0);
	printf("Alarm\n");

	while(1)
	{
		schedule_processes(signal_set_ALRM, sig);

		if(program[CurrentProcess].status_state == Running) // on the first iteration through this logic will not do anything bc they are all either stopped or exited right before the while loop is entered
		{

			if (check_if_currentproc_exited(program[CurrentProcess].PID) == 1)
			{
				//fprintf(stderr, "Exited %d\n", CurrentProcess);
				program[CurrentProcess].status_state = Exited;
				printf("Exited?: 1\n");
				printf("Set State Done: %d\n", program[CurrentProcess].PID);
				ACTIVE--;
			}
			else
			{
				if (ACTIVE > 1) // to make sure you dont stop it if it is the last process to complete
				{
					//fprintf(stderr, "Stopped %d\n", CurrentProcess);
					printf("*************************\n");
					read_and_print_proc_stats(&program[CurrentProcess]);
					printf("*************************\n");
					printf("Stopping: %d\n", program[CurrentProcess].PID);
					send_signal(SIGSTOP, program[CurrentProcess]);
					program[CurrentProcess].status_state = Paused;
				}
				else
				{
					//fprintf(stderr, "Last Process, allowing to finish %d\n", CurrentProcess);
					ualarm(quantum, 0);
					continue; // goes to the next iteration of the loop
				}
			}

			CurrentProcess = ((CurrentProcess + 1) % NUM_PROCS); // this allows it to wrap back around in case I am at the last proc and need to go back to the first proc
		}

		// find next process to start

		int nextProc = 0;
		int i;

		for(i = CurrentProcess; i < (NUM_PROCS+CurrentProcess); i++)
		{
			if(program[i % NUM_PROCS].status_state == Paused)
			{
				nextProc = (i % NUM_PROCS);
				break; // I found the next proc so leave scheduling loop
			}
		}

		if (i == NUM_PROCS+CurrentProcess)
		{
			break; // because I made it to the end so leave scheduling loop
		}

		//fprintf(stderr, "Scheduling next process %d\n", nextProc);
		// I am at part 3 so I found the next process and I need to sigcont to this process and update current process and the state of it 

		printf("Resuming: %d\n", program[nextProc].PID);
		send_signal(SIGCONT, program[nextProc]);
		program[nextProc].status_state = Running;

		CurrentProcess = nextProc;

		//reschedule alarm
		ualarm(quantum, 0);
		printf("Alarm\n");
	}
	print_the_info(program);

}

void start_the_processes(Process * program)
{
	/*
	This function loops through the entire processes and starts each,
	then changes their status_state in the struct to Running. 
	*/	
	int i;
	for (i = 0; i < NUM_PROCS; i++)
	{
		// printf("Starting: %d\n", program[i].PID);
		send_signal(SIGUSR1, program[i]);
		program[i].status_state = Running;
	}
}

void stop_the_processes(Process * program)
{
	/*
	This function loops through the entire processes and stops each,
	then changes their status_state in the struct to Paused. 
	*/	
	int i;
	for (i = 0; i < NUM_PROCS; i++)
	{
		send_signal(SIGSTOP, program[i]);
		program[i].status_state = Paused;
	}
}

void exit_the_processes(Process * program)
{
	/*
	This function loops through the entire processes checks
	if that process has exited or not using the 
	check_if_currentproc_exited function which calles
	waitpid to check the status of the process. If it has
	exited, then the status of the process is changed to
	Exited and the number of active processes (global variable)
	is decremented. 
	*/	
	int i;
	for (i = 0; i < NUM_PROCS; i++)
	{
		if(check_if_currentproc_exited(program[i].PID) == 1)
		{
			program[i].status_state = Exited;
			ACTIVE--;
		}
	}	
}


void free_process(Process * my_process)
{
	/*
	Freeing the args and current_command in a single process
	struct. This function needs to be called in a loop through 
	all the processes in the program, and there will need to
	be an additional free line for the entire Process program
	struct at the end of main.
	*/
	free(my_process->args);
	free(my_process->current_command);
}

void print_the_info(Process * program)
{
	int i;
	int j;

	for (i = 0; i < NUM_PROCS; i++)
	{
		printf("Proc:\t\t%p\n", &program[i]);
		printf("\t\tCMD:\t%s\n", program[i].current_command);
		printf("\t\tArgs:\n");
		int arg_count = 0;
		arg_count = count_the_args(*program[i].args);
		for(j=0; j < arg_count+1; j++)
		{
			printf("\t\tArg:\t%s\n", program[i].args[j]);
		}
		printf("\t\tPID:\t%d\n", program[i].PID);
		if (program[i].status_state == 0)
		{
			printf("\t\tState:\tNotStarted\n");
		}
		if (program[i].status_state == 1)
		{
			printf("\t\tState:\tRunning\n");
		}
		if (program[i].status_state == 2)
		{
			printf("\t\tState:\tPaused\n");
		}
		if (program[i].status_state == 3)
		{
			printf("\t\tState:\tExited\n");
		}
		printf("\t\tNext:%p\n", &program[i]);
	}
}

void execute_all_processes(Process * program)
{
	int i;

	for (i = 0; i < NUM_PROCS; i++)
	{
		//fprintf(stderr, "In for loop before forking\n");
		// launching all of the processes
		program[i].PID = fork();
		print_the_info(program);

		printf("Starting");
		if (program[i].PID < 0)
		{
			printf("handle error\n");
		}
		if (program[i].PID == 0)
		{
			while(!RUN_VAR)
			{
				usleep(100);
			}

			execvp(program[i].current_command, program[i].args);
			puts("execvp failed\n");
			printf("%d\n", errno);
			exit(-1);
		}
	}

}

void read_and_print_proc_stats(Process * process)
{
	char st_filename[100];
	char current_state;

	FILE * status_file;

	snprintf(st_filename, 100, "/proc/%d/stat", process -> PID);
	
	printf("%s\n", st_filename);

	status_file = fopen(st_filename, "r");

	if (status_file == NULL)
	{
		printf("Error opening /proc file\n");
		return;
	}

	fscanf(status_file, "%*d %*s %c %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu ", &current_state, &process -> usertime, &process -> systemtime);
	// process id, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime(14), stime(15), cutime, cstime

	printf("\tProcess %d usertime: %lu\n\tProcess %d systemtime: %lu\n\tProcess %d state: %c.\n", process -> PID, process -> usertime, process -> PID, process -> systemtime, process -> PID, current_state);

}

int main(int argc, char* argv[])
{
	FILE * file_of_commands_input; // setting up the file 
	file_of_commands_input = fopen(argv[1], "r");
	if (file_of_commands_input == NULL)
	{
		//printf("Error opening file.\n");
		return 1;
	}

	int line_count = 0;
	line_count = count_file_lines(file_of_commands_input); // using int pointer
	NUM_PROCS = line_count; // could condense later
	ACTIVE = NUM_PROCS; // initializing the ACTIVE to be the number of processes

	fseek(file_of_commands_input, 0, SEEK_SET); // going back to the top of the file
	program = malloc_process_array(line_count, file_of_commands_input);
	fseek(file_of_commands_input, 0, SEEK_SET); // going back to the top of the file
	create_array_of_commands(file_of_commands_input, program);
	fclose(file_of_commands_input); // done with the input file

	int i;

	// setting up sigusr1
	struct sigaction SA;
	sigemptyset(&SA.sa_mask);
	SA.sa_flags = 0;
	SA.sa_handler = sig_handler;
	sigaction(SIGUSR1, &SA, 0);

	// setting up the set for sigusr1
	int sig = 0;
	sigset_t signal_set_USR1;
	sigemptyset(&signal_set_USR1);
	sigaddset(&signal_set_USR1, SIGUSR1);

	// setting up the set for sigalarm
	sigset_t signal_set_ALRM;
	sigemptyset(&signal_set_ALRM);
	sigaddset(&signal_set_ALRM, SIGALRM);

	execute_all_processes(program); // launching all of the processes
	start_the_processes(program); // sends sigusr1 and change to running
	stop_the_processes(program); // sends sigstop and change to paused
	exit_the_processes(program); // going through and checking if they have exited and updating status_state
	schedule_entire_program(program, signal_set_ALRM, sig); // this is the following lines in a function

	for(i = 0; i < line_count; i++)
	{
		//wait(&program[i].PID);
		free_process(&program[i]);
	}

	free(program);
	return 0;
}
