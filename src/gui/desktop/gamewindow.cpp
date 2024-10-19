#include "gamewindow.h"

#include "ui_gamewindow.h"

/**
 * Constructor for GameWindow object.
 *
 * This constructor sets up the main window of the BrickGame application. It
 * initializes the UI, sets the window icon and title, and creates the start
 * screen, game over screen, and game field widgets. It also sets up a timer
 * to trigger the game loop.
 */
GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow) {
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/img/icon.png"));
  this->setWindowTitle("BrickGame");
  this->setFixedSize(600, 645);

  start_screen = new QWidget(this);
  setup_start_screen();

  gameover_screen = new QWidget(this);
  setup_gameover_screen();
  gameover_screen->hide();

  game_field = new GameField(this);
  setCentralWidget(game_field);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWindow::game_loop);
  timer->start(1);
  setFocusPolicy(Qt::StrongFocus);
  start_screen->raise();
  start_screen->show();
}

/**
 * Destructor for GameWindow object.
 */
GameWindow::~GameWindow() { delete ui; }

/**
 * Handles key press events for the GameWindow.
 */
void GameWindow::keyPressEvent(QKeyEvent *event) {
  int key = event->key();
  if (key == Qt::Key_Enter || key == Qt::Key_Return) {
    userInput(Start, false);
  } else if (key == Qt::Key_Up) {
    userInput(Up, false);
  } else if (key == Qt::Key_Down) {
    userInput(Down, false);
  } else if (key == Qt::Key_Left) {
    userInput(Left, false);
  } else if (key == Qt::Key_Right) {
    userInput(Right, false);
  } else if (key == Qt::Key_Q) {
    userInput(Terminate, false);
  } else if (key == Qt::Key_Space) {
    userInput(Action, true);
  } else if (key == Qt::Key_P) {
    userInput(Pause, false);
  } else {
    QMainWindow::keyPressEvent(event);
  }
}

/**
 * Handles key release events for the GameWindow.
 */
void GameWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    userInput(Action, false);
  } else {
    QMainWindow::keyReleaseEvent(event);
  }
}

/**
 * Sets up the start screen for the game window.
 * The start screen is shown when the game is first launched.
 */
void GameWindow::setup_start_screen() {
  start_screen->setFixedSize(600, 645);
  start_screen->setAutoFillBackground(true);
  QPalette pal;
  pal.setColor(QPalette::Window, QColorConstants::Svg::whitesmoke);
  start_screen->setPalette(pal);

  start_layout = new QVBoxLayout(start_screen);
  top_label = new QLabel(start_screen);
  top_label->setFixedSize(600, 100);
  top_label->setStyleSheet("font-size: 30px; font-weight: bold;");
  top_label->setText(TOP_TITLE);
  top_label->setAlignment(Qt::AlignCenter);
  start_layout->addWidget(top_label);

  img_label = new QLabel(start_screen);
  QPixmap pic;
  pic.load(START_IMG);
  img_label->setPixmap(pic);
  img_label->setAlignment(Qt::AlignCenter);
  start_layout->addWidget(img_label);

  bottom_label = new QLabel(start_screen);
  bottom_label->setFixedSize(600, 100);
  bottom_label->setStyleSheet("font-size: 18px; font-weight: bold;");
  bottom_label->setText("Press [Enter] to start game,\n[q] for exit");
  bottom_label->setAlignment(Qt::AlignCenter);
  start_layout->addWidget(bottom_label);

  start_screen->setLayout(start_layout);
}

/**
 * Sets up the game over screen for the GameWindow.
 * The game over screen is shown when the player loses or wins the game.
 */
void GameWindow::setup_gameover_screen() {
  gameover_screen->setFixedSize(250, 300);
  gameover_screen->move(330, 320);

  gameover_layout = new QVBoxLayout(gameover_screen);

  gameover_top_label = new QLabel(gameover_screen);
  gameover_top_label->setFixedSize(250, 40);
  gameover_top_label->setStyleSheet("font-size: 24px; font-weight: bold;");
  gameover_top_label->setAlignment(Qt::AlignCenter);
  gameover_layout->addWidget(gameover_top_label);

  gameover_img_label = new QLabel(gameover_screen);
  gameover_img_label->setAlignment(Qt::AlignCenter);
  gameover_layout->addWidget(gameover_img_label);

  gameover_bottom_label = new QLabel(gameover_screen);
  gameover_bottom_label->setFixedSize(250, 50);
  gameover_bottom_label->setStyleSheet("font-size: 18px; font-weight: bold;");
  gameover_bottom_label->setText("[Enter] - restart game\n[q] - exit");
  gameover_bottom_label->setAlignment(Qt::AlignCenter);
  gameover_layout->addWidget(gameover_bottom_label);

  gameover_screen->setLayout(gameover_layout);
}

/**
 * Updates the game over screen with the appropriate image and title based on
 * the game's outcome.
 */
void GameWindow::gameover_screen_update(int status) {
  QPixmap pic;
  if (status == GAMEOVER_) {
    pic.load(LOSE_PIC);
    gameover_top_label->setText(LOSE_TITLE);
  }
  if (status == WIN_) {
    pic.load(WIN_PIC);
    gameover_top_label->setText(WIN_TITLE);
  }
  gameover_img_label->setPixmap(pic);
}

/**
 * Runs the main game loop, updating the game state and UI elements.
 * It checks the current game state, updates the UI, and repaints the game field
 * and stats.
 */
void GameWindow::game_loop() {
  game_field->game_info = updateCurrentState();

  if (game_field->game_info.level == -1) this->close();
  if (game_field->game_info.pause == 1)
    game_field->pause_label->show();
  else
    game_field->pause_label->hide();
  if (game_field->game_info.pause == 0) {
    start_screen->hide();
    gameover_screen->hide();
    game_field->controls_label->show();
  }
  if (game_field->game_info.pause > 1) {
    game_field->controls_label->hide();
    gameover_screen->raise();
    gameover_screen_update(game_field->game_info.pause);
    gameover_screen->show();
  }
  game_field->score_label->setText(
      "SCORE:                    " +
      QString::number(game_field->game_info.score));
  game_field->high_score_label->setText(
      "HIGH SCORE:          " +
      QString::number(game_field->game_info.high_score));
  game_field->level_label->setText(
      "LEVEL:                     " +
      QString::number(game_field->game_info.level));
  game_field->speed_label->setText(
      "SPEED:                   " +
      QString::number(game_field->game_info.speed));

  game_field->repaint();
}
