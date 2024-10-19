#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "gamefield.h"

#ifdef SNAKE
#define TOP_TITLE "SNAKE"
#define START_IMG ":/img/snake.png"
#elif defined(TETRIS)
#define TOP_TITLE "TETRIS"
#define START_IMG ":/img/tetris.png"
#endif

#define LOSE_TITLE "GAME OVER"
#define WIN_TITLE "YOU WIN!"
#define LOSE_PIC ":/img/lose.png"
#define WIN_PIC ":/img/win.png"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow {
  Q_OBJECT

 public:
  friend class GameField;
  GameWindow(QWidget *parent = nullptr);
  ~GameWindow();

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void setup_start_screen();
  void setup_gameover_screen();
  void gameover_screen_update(int status);
  void game_loop();

 private:
  GameField *game_field;

  QWidget *start_screen;
  QVBoxLayout *start_layout;
  QLabel *top_label;
  QLabel *img_label;
  QLabel *bottom_label;

  QWidget *gameover_screen;
  QVBoxLayout *gameover_layout;
  QLabel *gameover_top_label;
  QLabel *gameover_img_label;
  QLabel *gameover_bottom_label;

  QTimer *timer;
  Ui::GameWindow *ui;
};
#endif  // GAMEWINDOW_H
