/*

Name: Alyssa Kelley

Due: Jan. 30th, 2019

Note: I worked on this assignment with Anne Glickenhause and Miguel N. No code was copied, just ideas/logic
was talked through.

Call this program like so:
gcc -std=c11 -g -o reversi.exe reversi.c testReversi.c

*/

#include "reversi.h"

int ask_gameboard_size(GameBoardContents *game_board)
{
    int game_board_size = 0;

    puts("Please enter size of board?");
    scanf("%d", &game_board_size);

    while (game_board_size % 2 != 0) // For my game board to function the best, I want only an even size board.
    {
        printf("Error: Please choose an even number for your board size.\n");
        puts("Please enter size of board?");
        scanf("%d", &game_board_size);
    }

    while (game_board_size > 10) // For my game board to function the best, the size should be less than 10.
    {
        printf("Error: Please choose a size 10 or less for youe game board.\n");
        puts("Please enter size of board?");
        scanf("%d", &game_board_size);
    }

    game_board -> board_size = game_board_size;
    return game_board -> board_size;

}

void allocate_memory_for_board(GameBoardContents *game_board)
{
    game_board -> board = malloc (sizeof(GameBoardContents) * game_board -> board_size * game_board -> board_size);
}

void initialize_game_board(GameBoardContents *game_board)
{
    int game_board_size = game_board -> board_size;
    //int game_board_array = game_board -> board;

    int index = 0;
    for(int i = 0; i < game_board_size; i++) {
        index = game_board_size*i; // this is for the row
        for(int j = 0; j < game_board_size; j++) {
            if((i==(game_board_size/2) && j==(game_board_size/2)) || (i==((game_board_size/2)-1) && j==((game_board_size/2)-1))) {
                //strcpy(&game_board[index+j], "#");
                game_board -> board [index+j] = PLAYER1;
                //game_board -> board [i][j] = '#';

            } else if((i==(game_board_size/2) && j==((game_board_size/2) - 1)) || (i==((game_board_size/2)-1) && j==(game_board_size/2))) { 
                //strcpy(&game_board[index+j], "$");
                game_board -> board[index+j] = PLAYER2;
                //game_board -> board[i][j] = '$';
            } else {
                //strcpy(&game_board[index+j], "O");
                game_board -> board[index+j] = 'O'; 
                //game_board -> board[i][j] = 'O';          
            }
        }
    }                     
}

void game_move_choice(GameBoardContents *game_board, Player *play)
{
    int row = 0;
    int col = 0;

    int game_board_size = game_board -> board_size;

    printf("Choose your position between 0-%d in the format of ROW:COL\n", game_board_size);
    scanf("%d:%d", &row, &col);
    //printf("%d:%d\n", row, col);


    while(0  > row || row > game_board_size || 0 > col || col > game_board_size)
    {
        printf("Invalid choice.\n");
        printf("Choose your position between 0-%d in the format of ROW:COL\n", game_board_size);
        scanf("%d:%d", &row, &col);
        printf("Player 1 = $ and Player 2 = @.\n");
    }

    if (play -> moves % 2 == 0)
    {
        play -> player_1_choice_row = row;
        play -> player_1_choice_col = col;
    }
    else
    {
        play -> player_2_choice_row = row;
        play -> player_2_choice_col = col;
    }
}

void move_piece(GameBoardContents *game_board, Player *play)
{
    int game_board_size = game_board -> board_size;

    int row = 0;
    int col = 0;

    if (play -> moves % 2 == 0) // This is player1 turn because the move player 1 = 1, player 2 = 2, player 1 = 3...
    {
        row = play -> player_1_choice_row - 1;
        col = play -> player_1_choice_col - 1;

        //game_board -> board[((game_board_size*row) + col)] = PLAYER1;


        /*
        This is checking this case: 
        0 $ @ 0 
        and wanting to change the col 4 position, and switch the col 3 position to player1 symbol.
        0 $ $ $ 
        so you have to check this by looping through 
        */

        if (game_board -> board[((game_board_size*row) + col - 1)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*row) + i)] == PLAYER2)
                    {
                        game_board -> board[((game_board_size*row) + i)] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;
        }
        /*
        This is checking this case: 
        0 $ @ 0 
        and wanting to change the col 1 position, and switch the col 3 position to player1 symbol.
        $ $ $ 0 
        */

        if (game_board -> board[((game_board_size*row) + col + 1)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*row) + i)] == PLAYER2)
                    {
                        game_board -> board[((game_board_size*row) + i)] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;

        }

        /*
        This is checking this case: 
        0 $ @ 0 
        0 @ $ 0
        0 0 0 0  
        to change it to this:
        0 $ @ 0 
        0 $ $ 0
        0 $ 0 0 
        */

        if (game_board -> board[((game_board_size* (row - 1)) + col)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*(row - i)) + col)] == PLAYER2)
                    {
                        game_board -> board[((game_board_size* (row - i)) + col)] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;

        }

        /*
        This is checking this case: 
        0 0 0 0 
        0 $ @ 0 
        0 @ $ 0

        to change it to this:
        0 $ 0 0  
        0 $ @ 0 
        0 $ $ 0

        */

        if (game_board -> board[((game_board_size* (row + 1)) + col)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*(row + i)) + col)] == PLAYER2)
                    {
                        game_board -> board[((game_board_size* (row + i)) + col)] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;

        }

        /*
        This is checking this case: 
        0 0 0 0 0 0 
        0 0 0 0 0 0
        0 0 $ @ 0 0
        0 0 @ $ 0 0
        0 0 0 0 0 0 
        0 0 0 0 0 0 

        to change it to this:
        0 0 0 0 0 0 
        0 0 0 0 $ 0
        0 0 $ $ 0 0
        0 0 $ $ 0 0
        0 $ 0 0 0 0 
        0 0 0 0 0 0 

        */

        if (game_board -> board[((game_board_size - 1) * row) + (col - 1)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size - i) * row) + (col - i))] == PLAYER2)
                    {
                        game_board -> board[(((game_board_size - i) * row) + (col - i))] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;
        }

        if (game_board -> board[((game_board_size + 1) * row) + (col - 1)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size + i) * row) + (col - i))] == PLAYER2)
                    {
                        game_board -> board[(((game_board_size + i) * row) + (col - i))] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;
        }

        if (game_board -> board[((game_board_size + 1) * row) + (col + 1)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size + i) * row) + (col + i))] == PLAYER2)
                    {
                        game_board -> board[(((game_board_size + i) * row) + (col + i))] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;
        }

        if (game_board -> board[((game_board_size - 1) * row) + (col + 1)] == PLAYER2)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size - i) * row) + (col + i))] == PLAYER2)
                    {
                        game_board -> board[(((game_board_size - i) * row) + (col + i))] = PLAYER1;
                        play -> player_1_points += 1;
                        play -> player_2_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER1;
            play -> player_1_points += 1;
        }

        // else
        // {
        //     printf("Invalid move. Player 2's turn.\n");
        // }
    }

    else 
    {
        row = play -> player_2_choice_row - 1; // because of the offset from the numbers printed out
        col = play -> player_2_choice_col - 1;

        if (game_board -> board[((game_board_size*row) + col - 1)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*row) + i)] == PLAYER1)
                    {
                        game_board -> board[((game_board_size*row) + i)] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;
        }

        if (game_board -> board[((game_board_size*row) + col + 1)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*row) + i)] == PLAYER1)
                    {
                        game_board -> board[((game_board_size*row) + i)] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;

        }

        if (game_board -> board[((game_board_size* (row - 1)) + col)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*(row - i)) + col)] == PLAYER1)
                    {
                        game_board -> board[((game_board_size* (row - i)) + col)] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;

        }

        if (game_board -> board[((game_board_size * (row + 1)) + col)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[((game_board_size*(row + i)) + col)] == PLAYER1)
                    {
                        game_board -> board[((game_board_size* (row + i)) + col)] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;

        }

        if (game_board -> board[((game_board_size - 1) * row) + (col - 1)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size - i) * row) + (col - i))] == PLAYER1)
                    {
                        game_board -> board[(((game_board_size - i) * row) + (col - i))] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;
        }

        if (game_board -> board[((game_board_size + 1) * row) + (col - 1)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size + i) * row) + (col - i))] == PLAYER1)
                    {
                        game_board -> board[(((game_board_size + i) * row) + (col - i))] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;
        }

        if (game_board -> board[((game_board_size + 1) * row) + (col + 1)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size + i) * row) + (col + i))] == PLAYER1)
                    {
                        game_board -> board[(((game_board_size + i) * row) + (col + i))] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;
        }

        if (game_board -> board[((game_board_size - 1) * row) + (col + 1)] == PLAYER1)
        {
            for (int i = 0; i < game_board_size; i++)
            {
                if (game_board -> board[(((game_board_size - i) * row) + (col + i))] == PLAYER1)
                    {
                        game_board -> board[(((game_board_size - i) * row) + (col + i))] = PLAYER2;
                        play -> player_2_points += 1;
                        play -> player_1_points -= 1;
                    }
            }
            game_board -> board[((game_board_size*row) + col)] = PLAYER2;
            play -> player_2_points += 1;
        }
    }
}


void play_the_game(GameBoardContents *game_board, Player *play)
{
    int game_board_size = game_board -> board_size;
    play -> player_1_points = 2;
    play -> player_2_points = 2;
    //play -> moves = 1;

    while( play -> moves != (game_board_size * game_board_size - 4))
    {
        printf("This is move #%d: \n", play->moves);

        game_move_choice(game_board, play);
        move_piece(game_board, play);
        print_game_board(game_board);

        play -> moves += 1;

        printf("Current point status -- Player 1: %d - Player 2: %d.\n", play -> player_1_points, play -> player_2_points);

    }

    if (play -> player_1_points > play -> player_2_points)
    {
        printf("Player 1: %d - Player 2: %d - Player 1 Wins!\n", play -> player_1_points , play -> player_2_points);
    }
    else
    {
        printf("Player 1: %d - Player 2: %d - Player 2 Wins!\n", play -> player_1_points , play -> player_2_points);
    }
}

void print_game_board(GameBoardContents *game_board) 
{
    // First Row
    int game_board_size = game_board -> board_size;

    printf(" ");
    for(int i = 1; i <= game_board_size; i++) {
       printf("%d ", i);
    }
    printf("\n");
    
    // Printing the pieces
    int index = 0;
    for(int i = 0; i < game_board_size; i++) {
        printf("%d ", i+1); 
        index = game_board_size*i;
        for(int j = 0; j < game_board_size; j++) {
            printf("%c ", game_board -> board[index+j]);
        }
        printf("\n");
    }
}

// int main()
// {
//     GameBoardContents *game_board;
//     game_board = malloc(sizeof(GameBoardContents));

//     Player *play;
//     play = malloc(sizeof(Player));


//     int game_board_size = 0;

//     ask_gameboard_size(game_board);

//     allocate_memory_for_board(game_board);

//     initialize_game_board(game_board);

//     print_game_board(game_board);

//     play_the_game(game_board, play);

// }