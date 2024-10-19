#include "gamefield.h"

/**
 * Constructor of GameField widget.
 */
GameField::GameField(QWidget *parent) : QWidget{parent} {
  field = new QWidget(this);
  field->setGeometry(20, 20, 300, 600);

  score_label = new QLabel(this);
  high_score_label = new QLabel(this);
  level_label = new QLabel(this);
  speed_label = new QLabel(this);
  next_label = new QLabel(this);
  pause_label = new QLabel(this);
  controls_label = new QLabel(this);

  init_labels();
}

/**
 * Initializes the labels for the game field, including score, high score,
 * level, speed, next piece, pause, and controls.
 */
void GameField::init_labels() {
  int y = 30;
  QVector<std::pair<QLabel *, QString>> labels = {
      {score_label, "SCORE:"},
      {high_score_label, "HIGH SCORE:"},
      {level_label, "LEVEL:"},
      {speed_label, "SPEED:"},
      {next_label, "NEXT:"},
      {pause_label, "~ PAUSE ~"},
      {controls_label,
       "      [ ↑ ]\n"
       "[←]       [→]  -  move\n"
       "      [ ↓ ]\n\n"
       "   [Space]    -  action\n\n"
       "       [p]     -  pause\n\n"
       "       [q]     -  exit"}};
  for (auto label : labels) {
    label.first->setGeometry(340, 350, 250, 40);
    label.first->setText(label.second);
    label.first->setStyleSheet("font-size: 18px; font-weight: bold;");
    label.first->move(340, y);
    y += 40;
  }
  controls_label->setGeometry(340, 350, 250, 200);
  controls_label->move(340, 390);
  pause_label->move(340, 335);
}

/**
 * Processes given color index into corresponding QColor.
 */
QColor GameField::get_color(int color) {
  QColor res;
  if (color == 1) res = QColorConstants::Svg::crimson;
  if (color == 2) res = Qt::darkGreen;
  if (color == 4) res = Qt::blue;
  if (color == 6) res = QColorConstants::Svg::lightseagreen;
  if (color == 8) res = QColorConstants::Svg::darkorange;
  if (color == 9) res = Qt::yellow;
  if (color == 10) res = QColorConstants::Svg::blueviolet;
  if (color == 11) res = QColorConstants::Svg::olivedrab;
  return res;
}

/**
 * Paints a single cell in the game field with a rounded filled given color
 * rectangle.
 */
void GameField::paint_cell(QRect rect, QColor color, QPainter &painter) {
  QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
  gradient.setColorAt(0, color.lighter(150));
  gradient.setColorAt(1, color.darker(150));

  QPainterPath path;
  path.addRoundedRect(rect, 5, 5);
  QPen pen;
  painter.setPen(pen);
  painter.fillPath(path, gradient);
  painter.drawPath(path);
}

/**
 * Paints the game field and the next piece preview.
 */
void GameField::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(field->geometry(), QColorConstants::Svg::dimgrey);
  painter.drawRect(field->geometry());

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      QRect square((j * CELL_SIZE + 20), (i * CELL_SIZE + 20), CELL_SIZE,
                   CELL_SIZE);
      if (game_info.field[i][j] != 0) {
        QColor color = get_color(game_info.field[i][j]);
        paint_cell(square, color, painter);
      }
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game_info.next[i][j] != 0) {
        QRect square((j * CELL_SIZE + 350), (i * CELL_SIZE + 245), CELL_SIZE,
                     CELL_SIZE);
        QColor color = get_color(game_info.next[i][j]);
        paint_cell(square, color, painter);
      }
    }
  }

  painter.end();
}
