#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "../brickgame_api.h"

#define SPEED_MIN 900

// tetramino colors
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_BLUE 4
#define COLOR_CYAN 6
#define COLOR_ORANGE 8
#define COLOR_YELLOW_ 9
#define COLOR_VIOLET 10

// game states
typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  PAUSE,
  EXIT_STATE
} GameState_t;

// tetramino figure
typedef struct {
  int view[4][4];
  int x;
  int y;
  char type;
  int rows;
  int cols;
} Tetramino;

// extended game information
typedef struct {
  int field[HEIGHT][WIDTH];
  Tetramino next;
  Tetramino current;
  long long timer;
  int user_input;
  bool key_hold;
  GameState_t state;
  GameInfo_t tetris;
} GameData;

GameData *update_data();
void finite_state_machine();
void start_state_actions(GameData *game);
void spawn_state_actions(GameData *game);
void moving_state_actions(GameData *game);
void shifting_state_actions(GameData *game);
void attaching_state_actions(GameData *game);
void gameover_state_actions(GameData *game);
void pause_state_actions(GameData *game);

void stats_init(GameData *game);
long long int get_time();
void reset_field(GameData *game);
void free_field();

void reset_figure(Tetramino *figure);
void generate_figure(Tetramino *figure);
void spawn_figure();
void moving_left();
void moving_right();
void moving_down();
void rotate_figure();
void set_figure_on_field();

int leaving_field();
int collision();
int figure_overlay();

int remove_lines(int *lines);
void shift_lines(int line);

void calculate_score();
void set_level();
void save_high_score(int high_score);
int load_high_score();

#endif