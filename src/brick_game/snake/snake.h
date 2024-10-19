#ifndef SNAKE_H
#define SNAKE_H

#include <sys/time.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <queue>

#include "../brickgame_api.h"

// game colors
#define SNAKE_COLOR 11
#define HEAD_COLOR 6
#define APPLE_COLOR 1
#define FILLED 2

// snake parameters
#define START_LENGTH 4
#define MAX_LENGTH 200
#define SPEED_MIN 550
#define WIN_POINTS HEIGHT* WIDTH - START_LENGTH - 1

namespace s21 {
// game states
typedef enum { START = 0, MOVING, GAMEOVER, PAUSE, EXIT_STATE } GameState_t;

// snake moving directions
typedef enum { UP = 0, DOWN, LEFT, RIGHT } Moving;

class Game {
 public:
  ~Game();
  static Game* instance();
  void update_field();
  void get_action(UserAction_t act, bool hold);
  void finite_state_machine();

  GameInfo_t game_info;

 protected:
#ifdef TEST
 public:
#endif
  struct coords {
    int x = 0;
    int y = 0;
  };

  struct Apple {
    coords pos;
    int color = APPLE_COLOR;
  };

  struct Snake {
    std::deque<std::pair<int, coords>> body;
    Moving mov = UP;
    Moving next_mov = UP;
    bool overeated = false;
  };

  Game();
  Game(Game const&) = delete;
  Game& operator=(Game const&) = delete;

  void save_high_score(int high_score);
  void load_high_score();
  bool collision();
  void generate_apple();
  void generate_snake();
  bool eat_apple();
  void move_snake();
  void get_direction(UserAction_t action);
  void timer_processing();
  void update_speed_and_level();
  void update_highscore();
  void clear_field();
  void create_field();
  void delete_field();
  void stats_init();
  long long int get_time();
  void boost_snake();

  Snake snake;
  Apple apple;
  GameState_t state;
  int user_input;
  bool key_hold;
  long long int timer = 0;
};

}  // namespace s21
#endif