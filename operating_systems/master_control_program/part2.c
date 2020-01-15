/*

Authorship Statement

Name: Alyssa Kelley 
DUCK ID: alyssak
CIS 415 Project 1

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus (classmate), Quinn Milionis (classmate), Luke Peyralans (fellow CS student at UO),
and Zack Bower (fellow CS student at UO). 

*/

#include "part2.h"

int count_the_args(char * current_line)
{ // ask if single space
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
	This will malloc for the cluster of processes which we are 
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
	}

	return my_process; // this is pointing to a bunch of processes (all the lines from input)
}

void parse_the_line(Process * command_line, char * line)
{
	/*
	This will pull the command out from the line, and then it will
	create the arguments for that command. The Process struct 
	will keep track of all this.
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

void start_the_processes(Process * program, int line_count)
{
	/*
	This function loops through the entire processes and starts each one
	to start off.
	*/	
	int i;
	for (i = 0; i < line_count; i++)
	{
		send_signal(SIGUSR1, program[i]);
	}
}

void stop_the_processes(Process * program, int line_count)
{
	/*
	This function loops through the entire processes and stops each one
	after they were started.
	*/	
	int i;
	for (i = 0; i < line_count; i++)
	{
		send_signal(SIGSTOP, program[i]);
	}
}


void continue_the_processes(Process * program, int line_count)
{
	/*
	This function loops through the entire processes and continues each one
	after they were stopped.
	*/	
	int i;
	for (i = 0; i < line_count; i++)
	{
		send_signal(SIGCONT, program[i]);
	}
}

void execute_all_processes(Process * program, int line_count)
{
	int i;
	for (i = 0; i < line_count; i++)
	{
		//fprintf(stderr, "In for loop before forking\n");
		// launching all of the processes
		program[i].PID = fork();

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
			//fprintf(stderr,"Execvp\n");

			// printf("Proc:\t%p\n", &program[i]);
			// printf("\t\tCMD:\n%s\n", program[i].current_command);
			// printf("\t\tArgs:\n");
			// for(j=0; j < len(program[i].args); j++)
			// {
			// 	printf("\t\tArg:\t\n%s\n", program[i].args[j]);
			// }
			// printf("\t\tPID:\t%d\n", program[i].PID);
			// printf("\tState:\t%d\n", program[i].status_state);

			execvp(program[i].current_command, program[i].args);
			puts("execvp failed\n");
			printf("%d\n", errno);
			exit(-1);
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


int main(int argc, char* argv[])
{
	FILE * file_of_commands_input;
	Process * program;
	file_of_commands_input = fopen(argv[1], "r");
	if (file_of_commands_input == NULL)
	{
		//printf("Error opening file.\n");
		return 1;
	}
	int line_count = 0;
	line_count = count_file_lines(file_of_commands_input);

	fseek(file_of_commands_input, 0, SEEK_SET);

	program = malloc_process_array(line_count, file_of_commands_input);

	fseek(file_of_commands_input, 0, SEEK_SET);
	create_array_of_commands(file_of_commands_input, program);
	fclose(file_of_commands_input);

	int i;
	// setting up sigusr1
	struct sigaction SA;
	SA.sa_flags = 0;
	sigemptyset(&SA.sa_mask);
	SA.sa_handler = sig_handler;
	sigaction(SIGUSR1, &SA, 0);

	execute_all_processes(program, line_count);
	start_the_processes(program, line_count);
	stop_the_processes(program, line_count);
	continue_the_processes(program, line_count);

	for(i = 0; i < line_count; i++)
	{
		wait(&program[i].PID);
		free_process(&program[i]);
	}

	free(program);
	return 0;

}

