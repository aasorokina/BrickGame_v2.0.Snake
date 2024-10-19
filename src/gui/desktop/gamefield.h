#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>
#include <QVBoxLayout>
#include <QWidget>

#include "../../brick_game/brickgame_api.h"

#define CELL_SIZE 30

class GameField : public QWidget {
  Q_OBJECT
 public:
  friend class GameWindow;
  explicit GameField(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void init_labels();
  QColor get_color(int color);
  void paint_cell(QRect rect, QColor color, QPainter &painter);
  GameInfo_t game_info;

 private:
  QWidget *field;

  QLabel *score_label;
  QLabel *high_score_label;
  QLabel *level_label;
  QLabel *speed_label;
  QLabel *next_label;
  QLabel *pause_label;
  QLabel *controls_label;
};

#endif  // GAMEFIELD_H
