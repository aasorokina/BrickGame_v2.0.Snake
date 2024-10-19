#include "brickgame.h"

int main(void) {
  ncurses_init();
  game_loop();
  endwin();

  return 0;
}

/**
 * The main game loop that runs the BrickGame.
 */
void game_loop() {
  UserAction_t input;
  do {
    erase();
    print_game_screen(updateCurrentState());
    userInput(input = get_action(getch()), false);
    refresh();
  } while (input != Terminate);
}

/**
 * Convert a user input key code to the corresponding user action.
 * @param user_input The key code of the user input.
 * @return The user action corresponding to the input key code.
 */
UserAction_t get_action(int user_input) {
  UserAction_t res;
  switch (user_input) {
    case KEY_UP:
      res = Up;
      break;
    case KEY_DOWN:
      res = Down;
      break;
    case KEY_LEFT:
      res = Left;
      break;
    case KEY_RIGHT:
      res = Right;
      break;
    case SPACE_KEY:
      res = Action;
      break;
    case PAUSE_KEY:
      res = Pause;
      break;
    case ESCAPE_KEY:
      res = Terminate;
      break;
    case ENTER_KEY:
      res = Start;
      break;
  }
  return res;
}