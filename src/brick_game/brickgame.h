#ifndef BRICKGAME_H
#define BRICKGAME_H

#include "../gui/cli/brickgame_cli.h"

#ifdef TETRIS
#include "tetris/tetris.h"
#elif defined (SNAKE)
#include "snake/snake.h"
#endif

void game_loop();
UserAction_t get_action(int user_input);

#endif