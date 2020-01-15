#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int board_size;
	char *board;
} GameBoardContents;

typedef struct {
	int moves; // counter

	int player_1_points;
	int player_1_choice_row;
	int player_1_choice_col;

	int player_2_points;
	int player_2_choice_row;
	int player_2_choice_col;

} Player;

	
enum designTile { PLAYER1 = '$', PLAYER2 = '@' } tilesymbol;

int ask_gameboard_size(GameBoardContents *game_board);

void allocate_memory_for_board(GameBoardContents *game_board);

void initialize_game_board(GameBoardContents *game_board);

void game_move(GameBoardContents *game_board);

void play_the_game(GameBoardContents *game_board, Player *play);

void print_game_board(GameBoardContents *game_board);
