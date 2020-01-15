
#include <stdio.h>
#include <stdlib.h>

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

// GOING THROUGH EACH NUMBER STARTING AT 0 (POS) OR 1 (NEG) 
// can be 48-57 in ASCII

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
    char *number1 = argv[1];
    char *operation = argv[2];
    char *number2 = argv[3];
    
    int total_number;
    int num_1;
    int num_2;

    num_1 = turn_into_int(number1); /* Calling my function up above to make this into an int */

    num_2 = turn_into_int(number2);

    if (argc != 4)
    {
        printf("Usage: %s <integer> <+ or -> <integer>", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (operation[0] == 43)  /* This is the ASCII representation for + and - */
    {
        total_number = (num_1 + num_2);
    }
    else if (operation[0] == 45)
    {
        total_number = (num_1 - num_2);
    }

    else if ((operation[0] != 43) || (operation[0] != 45))
    {
        printf("ERROR: operation may only be + or -\n");
        exit(EXIT_FAILURE);

    }

    if ((operation[1]) != '\0')
    {
        printf("ERROR: operation may only be + or -\n");
        exit(EXIT_FAILURE);
    }

    printf("%d\n",total_number);
    return 0;
}
/* 
TEST CASES:

./proj2e 4 + 8                                  -> 12                           SUCCESS
./proj2e 123 - 123                              -> 0                            SUCCESS
 ./proj2e 123 - -123                            -> 246                          SUCCESS
./proj2e 123 + 123                              -> 246                          SUCCESS
./proj2e 123 + -123                             -> 0                            SUCCESS
./proj2e 123 + 1234                             -> 1357                         SUCCESS
./proj2e 12334 - 789234                         -> -776900                      SUCCESS
./proj2e 12sd334 - -789234                      -> ERROR: 0-9                   SUCCESS
./proj2e 123312323494 - 789234                  -> ERROR: 10 digits             SUCCESS
./proj2e 7 -- 789234                            -> ERROR: opperation            SUCCESS
./proj2e 123394 ! 789234                        -> ERROR: opperation            SUCCESS

*/