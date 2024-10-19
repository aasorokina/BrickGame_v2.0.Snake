#include "brickgame_cli.h"

/**
 * Init ncurses and set screen parameters.
 */
void ncurses_init() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  srand(time(NULL));
  nodelay(stdscr, TRUE);
  init_colors();
}

/**
 * Print game screen based on the current game state in console.
 */
void print_game_screen(GameInfo_t game) {
  print_main_frame();
  switch (game.pause) {
    case (START_):
      print_start_screen();
      break;
    case (PAUSE_):
      print_playing_field_frame();
      print_pause();
      print_field(game);
      print_stats(game);
      break;
    case (GAMEOVER_):
      print_playing_field_frame();
      print_field(game);
      print_game_over(game);
      print_loose();
      break;
    case (WIN_):
      print_playing_field_frame();
      print_field(game);
      print_game_over(game);
      print_win();
      break;
    default:
      print_playing_field_frame();
      print_field(game);
      print_stats(game);
      break;
  }
}

void print_playing_field_frame() {
  print_box(1, F_Y_START + HEIGHT, 2, F_X_START + WIDTH * CELL_SIZE);
}

void print_main_frame() {
  print_box(0, F_Y_START + HEIGHT + 1, 0,
            F_X_START + (WIDTH * CELL_SIZE) * 2 + 6);
}

void print_start_screen() {
  mvprintw(4, 7, "______ ______  _____  _____  _   __ ");
  mvprintw(5, 7, "| ___ \\| ___ \\|_   _|/  __ \\| | / / ");
  mvprintw(6, 7, "| |_/ /| |_/ /  | |  | /  \\/| |/ /  ");
  mvprintw(7, 7, "| ___ \\|    /   | |  | |    |    \\  ");
  mvprintw(8, 7, "| |_/ /| |\\ \\  _| |_ | \\__/\\| |\\  \\ ");
  mvprintw(9, 7, "\\____/ \\_| \\_| \\___/  \\____/\\_| \\_/ ");
  mvprintw(10, 7, "        _____   ___  ___  ___ _____");
  mvprintw(11, 7, "       |  __ \\ / _ \\ |  \\/  ||  ___|");
  mvprintw(12, 7, "       | |  \\// /_\\ \\| .  . || |__  ");
  mvprintw(13, 7, "       | | __ |  _  || |\\/| ||  __| ");
  mvprintw(14, 7, "       | |_\\ \\| | | || |  | || |___ ");
  mvprintw(15, 7, "        \\____/\\_| |_/\\_|  |_/\\____/ ");

  attron(COLOR_PAIR(ORANGE_P));
  mvprintw(12, 6, "[]");
  mvprintw(13, 4, "[][][]");
  attroff(COLOR_PAIR(ORANGE_P));

  attron(COLOR_PAIR(CYAN_P));
  mvprintw(16, 9, "[][]");
  mvprintw(17, 7, "[][]");
  attroff(COLOR_PAIR(CYAN_P));

  attron(COLOR_PAIR(RED_P));
  mvprintw(2, 31, "()");
  attroff(COLOR_PAIR(RED_P));

  attron(COLOR_PAIR(L_GREEN_P));
  for (int i = 35; i <= 43; i += 2) mvprintw(2, i, "()");
  for (int i = 3; i <= 6; i++) mvprintw(i, 43, "()");
  for (int i = 6; i <= 9; i++) mvprintw(i, 45, "()");
  attroff(COLOR_PAIR(L_GREEN_P));

  attron(A_BLINK);
  mvprintw(HEIGHT, (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 2 - 9,
           "ENTER - start game");
  mvprintw(HEIGHT + 1, (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 2 - 9,
           "    q - exit");
  attroff(A_BLINK);
}

void print_pause() {
  attron(A_BLINK);
  mvprintw(F_Y_START + 12, F_X_START + WIDTH * CELL_SIZE + 3, "PAUSE");
  attroff(A_BLINK);
}

void print_box(int top_y, int bottom_y, int left_x, int right_x) {
  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }
  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(top_y, i, ACS_HLINE);
    mvaddch(bottom_y, i, ACS_HLINE);
  }
  mvaddch(top_y, left_x, ACS_ULCORNER);
  mvaddch(top_y, right_x, ACS_URCORNER);
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
}

void print_field(GameInfo_t game) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game.field[i][j] != 0) {
        attron(COLOR_PAIR(game.field[i][j]));
        mvprintw(F_Y_START + i, F_X_START + j * CELL_SIZE, CELL);
        attroff(COLOR_PAIR(game.field[i][j]));
      }
    }
  }
}

void print_stats(GameInfo_t game) {
  mvprintw(F_Y_START, F_X_START + WIDTH * CELL_SIZE + 3, "SCORE: %d",
           game.score);
  mvprintw(F_Y_START + 1, F_X_START + WIDTH * CELL_SIZE + 3, "HIGH SCORE: %d",
           game.high_score);
  mvprintw(F_Y_START + 3, F_X_START + WIDTH * CELL_SIZE + 3, "LEVEL: %d",
           game.level);
  mvprintw(F_Y_START + 5, F_X_START + WIDTH * CELL_SIZE + 3, "SPEED: %d",
           game.speed);
  mvprintw(F_Y_START + 7, F_X_START + WIDTH * CELL_SIZE + 3, "NEXT:");
  print_next(game, F_Y_START + 9, F_X_START + WIDTH * CELL_SIZE + 3);
  mvprintw(F_Y_START + 14, F_X_START + WIDTH * CELL_SIZE + 3, "  ^");
  mvprintw(F_Y_START + 15, F_X_START + WIDTH * CELL_SIZE + 3, "<   >  -  move");
  mvprintw(F_Y_START + 16, F_X_START + WIDTH * CELL_SIZE + 3, "  V");
  mvprintw(F_Y_START + 17, F_X_START + WIDTH * CELL_SIZE + 3,
           "SPACE  -  action");
  mvprintw(F_Y_START + 18, F_X_START + WIDTH * CELL_SIZE + 3,
           "  p    -  pause");
  mvprintw(F_Y_START + 19, F_X_START + WIDTH * CELL_SIZE + 3, "  q    -  exit");
}

void print_next(GameInfo_t game, int y, int x) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.next[i][j] != 0) {
        attron(COLOR_PAIR(game.next[i][j]));
        mvprintw(y + i, x + j * CELL_SIZE, CELL);
        attroff(COLOR_PAIR(game.next[i][j]));
      }
    }
  }
}

void print_game_over(GameInfo_t game) {
  mvprintw(F_Y_START, F_X_START + WIDTH * CELL_SIZE + 3, "SCORE: %d",
           game.score);
  mvprintw(F_Y_START + 2, F_X_START + WIDTH * CELL_SIZE + 3, "HIGH SCORE: %d",
           game.high_score);
  attron(A_BLINK);
  mvprintw(18, F_X_START + WIDTH * CELL_SIZE + 3, "PLAY AGAIN?");
  attroff(A_BLINK);
  mvprintw(20, F_X_START + WIDTH * CELL_SIZE + 3, "ENTER  -  YES");
  mvprintw(21, F_X_START + WIDTH * CELL_SIZE + 3, "  q    -  NO");
}

void print_win() {
  attron(COLOR_PAIR(BLUE_P));
  mvprintw(6, F_X_START + WIDTH * CELL_SIZE + 3, "[YOU WIN!]");
  attroff(COLOR_PAIR(BLUE_P));
  attron(COLOR_PAIR(CYAN_P));
  mvprintw(8, F_X_START + WIDTH * CELL_SIZE + 3, "  .    .''.    *");
  mvprintw(9, F_X_START + WIDTH * CELL_SIZE + 3, "      :_\\/_:   ");
  mvprintw(10, F_X_START + WIDTH * CELL_SIZE + 3, "  .''.: /\\ :   ");
  mvprintw(11, F_X_START + WIDTH * CELL_SIZE + 3, " :_\\/_:'.:::.  ");
  mvprintw(12, F_X_START + WIDTH * CELL_SIZE + 3, " : /\\ : :::::  ");
  mvprintw(13, F_X_START + WIDTH * CELL_SIZE + 3, "  '..'  ':::'   ");
  mvprintw(14, F_X_START + WIDTH * CELL_SIZE + 3, "     *    '     ");
  mvprintw(15, F_X_START + WIDTH * CELL_SIZE + 3, "      *  '      ");
  mvprintw(16, F_X_START + WIDTH * CELL_SIZE + 3, "      *         ");
  attroff(COLOR_PAIR(CYAN_P));
}

void print_loose() {
  attron(COLOR_PAIR(BLUE_P));
  mvprintw(6, F_X_START + WIDTH * CELL_SIZE + 3, "[GAME OVER]");
  attroff(COLOR_PAIR(BLUE_P));
  attron(COLOR_PAIR(ORANGE_P));
  mvprintw(8, F_X_START + WIDTH * CELL_SIZE + 3, "            ____  ");
  mvprintw(9, F_X_START + WIDTH * CELL_SIZE + 3, "         / >     >");
  mvprintw(10, F_X_START + WIDTH * CELL_SIZE + 3, "        |   _   _|");
  mvprintw(11, F_X_START + WIDTH * CELL_SIZE + 3, "        / == _x ==");
  mvprintw(12, F_X_START + WIDTH * CELL_SIZE + 3, "       /         |");
  mvprintw(13, F_X_START + WIDTH * CELL_SIZE + 3, "      /  \\      / ");
  mvprintw(14, F_X_START + WIDTH * CELL_SIZE + 3, "   / ￣|  |  |  | ");
  mvprintw(15, F_X_START + WIDTH * CELL_SIZE + 3, "  | (_￣\\__\\_)_) ");
  mvprintw(16, F_X_START + WIDTH * CELL_SIZE + 3, "   \\__)");
  attroff(COLOR_PAIR(ORANGE_P));
}

/**
 * Initialize the color palette and color pairs used in the BrickGame.
 */
void init_colors() {
  start_color();
  init_color(COLOR_ORANGE, 800, 429, 0);
  init_color(COLOR_YELLOW_, 900, 900, 0);
  init_color(COLOR_VIOLET, 442, 100, 800);
  init_color(COLOR_L_GREEN, 234, 777, 515);
  init_pair(RED_P, COLOR_RED, COLOR_BLACK);
  init_pair(ORANGE_P, COLOR_ORANGE, COLOR_BLACK);
  init_pair(YELLOW_P, COLOR_YELLOW_, COLOR_BLACK);
  init_pair(GREEN_P, COLOR_GREEN, COLOR_BLACK);
  init_pair(CYAN_P, COLOR_CYAN, COLOR_BLACK);
  init_pair(BLUE_P, COLOR_BLUE, COLOR_BLACK);
  init_pair(VIOLET_P, COLOR_VIOLET, COLOR_BLACK);
  init_pair(L_GREEN_P, COLOR_L_GREEN, COLOR_BLACK);
}