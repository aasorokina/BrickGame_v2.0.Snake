#ifndef BRICKGAME_API_H
#define BRICKGAME_API_H

#include <stdbool.h>

// game parameters
#define HEIGHT 20
#define WIDTH 10
#define LEVEL_MAX 10
#define LEVEL_MIN 1

// user input keys
#define ESCAPE_KEY 'q'
#define ENTER_KEY 10
#define SPACE_KEY 32
#define PAUSE_KEY 'p'

// screen game states
typedef enum { START_ = -1, PLAYING_, PAUSE_, GAMEOVER_, WIN_ } ScreenState;

// user actions
typedef enum {
  Start = 0,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

// main game info
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#ifdef __cplusplus
extern "C" {
#endif

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

#ifdef __cplusplus
}
#endif

#endif