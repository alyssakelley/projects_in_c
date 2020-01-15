// Author: Alyssa Kelley
// Project 3B - continuation of 2E
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 10 // this is for int int operation

typedef struct
{
    // data member #1 
    // data member #2 = array
    int counter;
    int current_stack[MAX_STACK_SIZE]; // for the equation, could be 2 for just the ints in it

} Stack;

void Initialize(Stack *s)
{
    s -> counter = 0;
}

void Push(Stack *s, int x)
{
    // Two lines of code, with 5 more lines for error checking
    if ((s -> counter) == MAX_STACK_SIZE)
    {
        printf("Stack is full!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        s -> current_stack[(s -> counter)] = x;
        (s -> counter)++;
    }
    // printf("I am pushing %d\n", x);
}

int Pop(Stack *s)
{
    // Two lines of code, with 5 more lines for error checking
    --(s -> counter);
    //printf("I am pushing %d\n", s -> current_stack[(s -> counter)]);
    return s -> current_stack[(s -> counter)];

}

int turn_into_int(char *string_number)
{
    int digit;
    int entire_integer = 0; /* initializing as 0 */
    int sign_of_int = 1; /* defaulting it to postive unless otherwise changed. */
    int starting_digit; /* this will be 0 if number is pos, and 1 if number is neg */
    int first_digit = string_number[0] - '0';

// RE-CREATING STRLEN
    int len = 0; 
    char * c = string_number;
    while(*c != 0)
    {
        len++;
        c++;
    }
    if (len > 10)
    {
        printf("ERROR: number has 10 or more digits\n");
        exit(EXIT_FAILURE);
    }

// CHECKING SIGN OF NUMBER

    if (first_digit < 0) /* this means it is negative */
    {
        sign_of_int = -1;
        starting_digit = 1;
    }
    else /* this means it is positive */
    {
        starting_digit = 0;
    }

    for (digit = starting_digit; digit < 10; digit++)
    {
        if (string_number[digit] != '\0')
        {
            int string_as_int = string_number[digit] - '0'; /* Converting to int */
            entire_integer = ((entire_integer * 10) + string_as_int);
            if ((48 > string_number[digit]) || (string_number[digit] > 57))
            {
            printf("ERROR: number has character that is outside 0-9\n");
            exit(EXIT_FAILURE);
            }
        }
        else if (string_number[digit] == '\0')
        {
            return (entire_integer * sign_of_int);
        } 
        // checking length of int
        else if (digit >= 10)
        {
            printf("ERROR: number has 10 or more digits\n");
            exit(EXIT_FAILURE);
        }
    }
    return entire_integer;
}
int main(int argc, char *argv[])
{
    //char *number1 = argv[1];
    // char *operation = argv[2];
    // char *number2 = argv[3];
    int answer;
    int num_1;
    int num_2;
    Stack s;
    Initialize(&s);

    for (int i=1; i < argc; i++) // going through each argv
    {
        char *str = argv[i]; 
        // address of s creates the pointer.
        if (str[0] == '+' || str[0] == '-' || str[0] == 'x')
        {
            num_2 = Pop(&s); //popping off the most recently pushed int
            //printf("This is my num_2 popped off: %d\n", num_2);

            num_1 = Pop(&s); //popping off the first pushed int
            //printf("This is my num_1 popped off: %d\n", num_1);

            if (str[0] == '+')
            {
                answer = (num_1 + num_2);
                //printf("This is %d + %d = %d\n", num_1, num_2, answer);
            }

            else if (str[0] == '-')
            {
                answer = (num_1 - num_2);
                //printf("This is %d - %d = %d\n", num_1, num_2, answer);
            }    

            else if (str[0] == 'x')
            {
                answer = (num_1 * num_2);
                //printf("This is %d * %d = %d\n", num_1, num_2, answer);
            }
            Push(&s, answer);
        }

        else
        {
            // turn into int
            int number = turn_into_int(&str[0]);
            //printf("This is my number: %d\n", number);
            Push(&s, number); // pushing the int to the stack
        }
    }
    // if ((operation[1]) != '\0')
    // {
    //     printf("ERROR: operation may only be + or -\n");
    //     exit(EXIT_FAILURE);
    // }
    printf("The total is %d\n",answer);
    return 0;
}