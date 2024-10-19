#ifndef BRICKGAME_CLI_H
#define BRICKGAME_CLI_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../brick_game/brickgame_api.h"

// start points for the playing field
#define F_Y_START 2
#define F_X_START 3

// tetramino cells
#ifdef TETRIS
#define CELL "[]"
#elif defined SNAKE
#define CELL "()"
#endif
#define CELL_SIZE strlen(CELL)

// custom colors
#define COLOR_ORANGE 8
#define COLOR_YELLOW_ 9
#define COLOR_VIOLET 10
#define COLOR_L_GREEN 11

// color pairs
#define RED_P 1
#define ORANGE_P 8
#define YELLOW_P 9
#define GREEN_P 2
#define CYAN_P 6
#define BLUE_P 4
#define VIOLET_P 10
#define L_GREEN_P 11

void ncurses_init();
void init_colors();
void init_start_screen_figures();
void print_playing_field_frame();
void print_main_frame();
void print_field(GameInfo_t game);
void print_box(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(GameInfo_t game);
void print_next(GameInfo_t game, int y, int x);
void print_start_screen();
void print_pause();
void print_game_screen(GameInfo_t game);
void print_game_over(GameInfo_t game);
void print_win();
void print_loose();

#endif