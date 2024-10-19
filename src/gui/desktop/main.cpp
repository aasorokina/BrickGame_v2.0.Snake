#include <stdlib.h>
#include <time.h>

#include "gamewindow.h"

#ifdef SNAKE
#include "../../brick_game/snake/snake.h"
#elif defined(TETRIS)
#include "../../brick_game/tetris/tetris.h"
#endif

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GameWindow window;

  window.show();
  return a.exec();
}
