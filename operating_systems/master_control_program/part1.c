/*

Authorship Statement

Name: Alyssa Kelley 
DUCK ID: alyssak
CIS 415 Project 1

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus (classmate), Quinn Milionis (classmate), Luke Peyralans (fellow CS student at UO),
and Zack Bower (fellow CS student at UO). 

*/

#include "part1.h"

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


void execute_all_processes(Process * program, int line_count)
{
	int i;
	for (i = 0; i < line_count; i++)
	{
		program[i].PID = fork();

		if (program[i].PID < 0)
		{
			printf("handle error\n");
		}
		if (program[i].PID == 0)
		{
			// printf("Trying to run %s -- ", program[i].current_command);
			// for(j = 0; j < program[i].arg_count; j++) { printf("* %s ", program[i].args[j]); };
			execvp(program[i].current_command, program[i].args);//name);//program[i].args);
			puts("execvp failed\n");
			printf("%d\n", errno);
			exit(-1);
		}
	}
}


void free_process(Process * my_process)
{
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
	line_count = count_file_lines(file_of_commands_input); // using int pointer
	fseek(file_of_commands_input, 0, SEEK_SET);
	// printf("Line count: %d\n", line_count);

	program = malloc_process_array(line_count, file_of_commands_input);
	fseek(file_of_commands_input, 0, SEEK_SET);
	create_array_of_commands(file_of_commands_input, program);
	fclose(file_of_commands_input);

	int i;
	execute_all_processes(program, line_count);

	for(i = 0; i < line_count; i++)
	{
		wait(&program[i].PID);
		free_process(&program[i]);
	}

	free(program);
	return 0;

}