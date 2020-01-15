#include "reversi.h"

int main()
{
    GameBoardContents *game_board;
    game_board = malloc(sizeof(GameBoardContents));

    Player *play;
    play = malloc(sizeof(Player));


    int game_board_size = 0;

    ask_gameboard_size(game_board);

    allocate_memory_for_board(game_board);

    initialize_game_board(game_board);

    print_game_board(game_board);

    play_the_game(game_board, play);

    free(game_board);

    free(play);

}