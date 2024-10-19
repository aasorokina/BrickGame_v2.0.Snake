#include "snake.h"

namespace s21 {

/**
 * Constructor for Game object, creates the game field and then initializes the
 * game stats.
 */
Game::Game() {
  create_field();
  stats_init();
}

/**
 * Destructor for Game object.
 */
Game::~Game() { delete_field(); }

/**
 * Returns a singleton instance of the Game class.
 */
Game* Game::instance() {
  static Game game{};
  return &game;
}

/**
 * Creates the game field and the next block preview field.
 */
void Game::create_field() {
  game_info.field = new int*[HEIGHT]();
  for (int i = 0; i < HEIGHT; i++) {
    game_info.field[i] = new int[WIDTH]();
  }

  game_info.next = new int*[4]();
  for (int i = 0; i < 4; i++) {
    game_info.next[i] = new int[4]();
  }
}

/**
 * Deletes the dynamically allocated memory for the game field and the next
 * block preview field.
 */
void Game::delete_field() {
  if (game_info.field != nullptr) {
    for (int i = 0; i < HEIGHT; i++) {
      delete[] game_info.field[i];
      game_info.field[i] = nullptr;
    }
    delete[] game_info.field;
    game_info.field = nullptr;
  }

  if (game_info.next != nullptr) {
    for (int i = 0; i < 4; i++) {
      delete[] game_info.next[i];
      game_info.next[i] = nullptr;
    }
    delete[] game_info.next;
    game_info.next = nullptr;
  }
}

/**
 * Initializes the game field, snake, apple, and various game statistics.
 */
void Game::stats_init() {
  srand(time(NULL));
  clear_field();
  generate_snake();
  generate_apple();
  game_info.level = LEVEL_MIN;
  game_info.score = 0;
  game_info.high_score = 0;
  load_high_score();
  game_info.speed = SPEED_MIN;
  game_info.pause = START_;
  user_input = -1;
  key_hold = false;
  timer = get_time();
  state = START;
}

/**
 * Generates the start snake body.
 */
void Game::generate_snake() {
  snake.body.clear();

  int start_x = WIDTH / 2;
  int start_y = (HEIGHT / 2) - (START_LENGTH / 2);

  while (snake.body.size() < START_LENGTH) {
    snake.body.push_back({SNAKE_COLOR, {start_x, start_y}});
    start_y++;
  }
  snake.body.front().first = HEAD_COLOR;
  snake.mov = UP;
  snake.next_mov = UP;
}

/**
 * Moves the snake accordingly to current direction and checks for collisions
 * and apple eating.
 */
void Game::move_snake() {
  coords head = snake.body.front().second;
  coords tail = snake.body.back().second;
  Moving prev_mov = snake.mov;

  if (snake.next_mov == UP && prev_mov != DOWN)
    snake.body.push_front({HEAD_COLOR, {head.x, --(head.y)}});
  if (snake.next_mov == DOWN && prev_mov != UP)
    snake.body.push_front({HEAD_COLOR, {head.x, ++(head.y)}});
  if (snake.next_mov == RIGHT && prev_mov != LEFT)
    snake.body.push_front({HEAD_COLOR, {++(head.x), head.y}});
  if (snake.next_mov == LEFT && prev_mov != RIGHT)
    snake.body.push_front({HEAD_COLOR, {--(head.x), head.y}});
  if (snake.overeated) {
    snake.body[1].first = FILLED;
    snake.overeated = false;
  } else
    snake.body[1].first = SNAKE_COLOR;
  snake.body.pop_back();
  snake.mov = snake.next_mov;

  if (eat_apple()) {
    snake.overeated = true;
    snake.body.push_back({SNAKE_COLOR, tail});
  }

  if (collision()) {
    snake.body.push_back({SNAKE_COLOR, tail});
    snake.body.pop_front();
    snake.body.front().first = HEAD_COLOR;
    state = GAMEOVER;
    game_info.pause = GAMEOVER_;
  }
}

/**
 * Updates the snake's next movement direction based on the user's action,
 * ensuring the new direction is valid.
 */
void Game::get_direction(UserAction_t action) {
  Moving curr_mov = snake.mov;
  Moving new_mov = curr_mov;
  if (action == Up && curr_mov != DOWN) new_mov = UP;
  if (action == Down && curr_mov != UP) new_mov = DOWN;
  if (action == Right && curr_mov != LEFT) new_mov = RIGHT;
  if (action == Left && curr_mov != RIGHT) new_mov = LEFT;
  snake.next_mov = new_mov;
}

/**
 * Updates the game field by clearing it, drawing the apple, and drawing the
 * snake.
 */
void Game::update_field() {
  if (game_info.field != nullptr) {
    clear_field();
    game_info.field[apple.pos.y][apple.pos.x] = apple.color;

    for (size_t i = 0; i < snake.body.size(); i++) {
      game_info.field[snake.body[i].second.y][snake.body[i].second.x] =
          snake.body[i].first;
    }

    game_info.next[0][0] = APPLE_COLOR;
    game_info.next[0][1] = APPLE_COLOR;
    game_info.next[1][0] = APPLE_COLOR;
    game_info.next[1][1] = APPLE_COLOR;
  }
}

/**
 * Handles the user's action input and updates the game state accordingly.
 */
void Game::get_action(UserAction_t act, bool hold) {
  this->user_input = act;
  if (act == Action) key_hold = hold;
}

/**
 * Checks if the snake has collided with the game boundaries or itself.
 */
bool Game::collision() {
  bool collision = false;
  coords head = snake.body.front().second;
  if (head.x < 0 || head.x > (WIDTH - 1) || head.y < 0 ||
      head.y > (HEIGHT - 1)) {
    collision = true;
  }
  for (size_t i = 1; i < snake.body.size(); i++) {
    if (head.x == snake.body[i].second.x && head.y == snake.body[i].second.y) {
      collision = true;
    }
  }
  return collision;
}

/**
 * Generates a new apple position on the game field, ensuring it does not
 * overlap with the snake's body.
 */
void Game::generate_apple() {
  apple.pos.y = rand() % HEIGHT;
  apple.pos.x = rand() % WIDTH;
  apple.color = APPLE_COLOR;

  for (size_t i = 0; i < snake.body.size(); i++) {
    coords s_pos = snake.body[i].second;
    if (s_pos.x == apple.pos.x && s_pos.y == apple.pos.y) {
      generate_apple();
    }
  }
}

/**
 * Checks if the snake eaten the apple and updates the game state accordingly.
 * @return true if the snake has eaten the apple, false otherwise.
 */
bool Game::eat_apple() {
  bool eat = false;
  coords head = snake.body.front().second;
  if (head.x == apple.pos.x && head.y == apple.pos.y) {
    eat = true;
    game_info.score++;
    if (game_info.score >= WIN_POINTS) {
      state = GAMEOVER;
      game_info.pause = WIN_;
    } else
      generate_apple();
  }
  return eat;
}

/**
 * Updates the high score and saves the new high score to a file.
 */
void Game::update_highscore() {
  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
    save_high_score(game_info.high_score);
  }
}

/**
 * Updates the game's speed and level based on the current score.
 */
void Game::update_speed_and_level() {
  game_info.level = game_info.score / 5 + 1;
  if (game_info.level > LEVEL_MAX) game_info.level = LEVEL_MAX;
  if (game_info.level != LEVEL_MIN)
    game_info.speed = SPEED_MIN - (game_info.level * 40);
}

/**
 * Saves the high score to file.
 */
void Game::save_high_score(int high_score) {
  std::ofstream file;
  file.open("high_score.txt");
  if (file.is_open()) file << high_score;
  file.close();
}

/**
 * Loads the high score from file.
 */
void Game::load_high_score() {
  std::ifstream file;
  file.open("high_score.txt");
  if (file.is_open()) file >> game_info.high_score;
  file.close();
}

void Game::clear_field() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game_info.field[i][j] = 0;
    }
  }
}

/**
 * Returns the current time in milliseconds.
 */
long long int Game::get_time() {
  struct timeval now;
  gettimeofday(&now, NULL);
  return (long long)now.tv_sec * 1000 + now.tv_usec / 1000;
}

/**
 * Processes the game timer and moves the snake if the timer has elapsed.
 */
void Game::timer_processing() {
  if ((get_time() - timer) >= (game_info.speed / (1 + key_hold))) {
    timer = get_time();
    move_snake();
  }
}

/**
 * Boosts the snake's speed by reducing the timer delay.
 */
void Game::boost_snake() {
  if (key_hold == false) timer -= (game_info.level * 40);
}

/**
 * Implements the finite state machine for the game logic.
 */
void Game::finite_state_machine() {
  switch (state) {
    case START:
      game_info.pause = START_;
      if (user_input == Start) state = MOVING;
      if (user_input == Terminate) state = EXIT_STATE;
      break;

    case MOVING:
      game_info.pause = PLAYING_;
      if (user_input == Up || user_input == Down || user_input == Right ||
          user_input == Left) {
        get_direction((UserAction_t)user_input);
      }
      if (user_input == Action) boost_snake();
      if (user_input == Pause) state = PAUSE;
      timer_processing();
      update_highscore();
      update_speed_and_level();
      if (user_input == Terminate) state = EXIT_STATE;
      break;

    case GAMEOVER:
      if (user_input == Start) {
        stats_init();
        state = MOVING;
      }
      if (user_input == Terminate) state = EXIT_STATE;
      break;

    case PAUSE:
      game_info.pause = PAUSE_;
      if (user_input == Pause) {
        state = MOVING;
        timer = get_time();
      }
      if (user_input == Terminate) state = EXIT_STATE;
      break;

    case EXIT_STATE:
      game_info.level = -1;
      delete_field();
      break;

    default:
      break;
  }
  user_input = -1;
}

}  // namespace s21

/**
 * Updates the current state of the game and returns the updated game
 * information.
 */
GameInfo_t updateCurrentState() {
  s21::Game* game = s21::Game::instance();
  game->finite_state_machine();
  game->update_field();
  return game->game_info;
}

/**
 * Processes user input actions and updates the game state accordingly.
 */
void userInput(UserAction_t action, bool hold) {
  s21::Game* game = s21::Game::instance();
  game->get_action(action, hold);
}