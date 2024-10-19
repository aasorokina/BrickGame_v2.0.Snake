#include "test_snake.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  int failed = RUN_ALL_TESTS();
  return failed;
}

TEST(Snake, stats_init) {
  Game* game = Game::instance();

  EXPECT_EQ(game->game_info.level, LEVEL_MIN);
  EXPECT_EQ(game->game_info.score, 0);
  EXPECT_EQ(game->game_info.speed, SPEED_MIN);
  EXPECT_EQ(game->game_info.pause, START_);
  EXPECT_FALSE(game->key_hold);
  EXPECT_EQ(game->state, START);

  EXPECT_FALSE(game->snake.body.empty());

  bool apple_found = false;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game->apple.pos.x == j && game->apple.pos.x == i) {
        apple_found = true;
        break;
      }
    }
    if (apple_found) break;
  }
  EXPECT_TRUE(apple_found);
}

TEST(Snake, move_snake) {
  Game* game = Game::instance();
  game->apple.pos.x = 7;
  game->apple.pos.y = 7;

  Game::coords initial_head = game->snake.body.front().second;
  int initial_length = game->snake.body.size();

  userInput(Start, true);
  game->finite_state_machine();
  EXPECT_EQ(game->state, MOVING);

  GameInfo_t info = updateCurrentState();
  EXPECT_EQ(info.field[7][7], APPLE_COLOR);

  userInput(Right, true);
  game->finite_state_machine();
  EXPECT_EQ(game->snake.next_mov, RIGHT);

  userInput(Pause, true);
  game->finite_state_machine();
  EXPECT_EQ(game->state, PAUSE);

  userInput(Pause, true);
  game->finite_state_machine();
  EXPECT_EQ(game->state, MOVING);

  game->move_snake();
  game->move_snake();

  Game::coords new_head = game->snake.body.front().second;
  EXPECT_EQ(new_head.x, initial_head.x + 2);
  EXPECT_EQ(new_head.y, initial_head.y);

  game->game_info.score = 199;
  userInput(Up, true);
  game->finite_state_machine();
  game->move_snake();

  EXPECT_EQ(game->snake.body.size(), initial_length + 1);
  EXPECT_EQ(game->snake.mov, UP);

  EXPECT_EQ(game->state, GAMEOVER);
  EXPECT_EQ(game->game_info.pause, WIN_);

  userInput(Start, true);
  game->finite_state_machine();

  game->snake.body.front().second = {-1, 0};
  game->move_snake();
  EXPECT_EQ(game->state, GAMEOVER);
  EXPECT_EQ(game->game_info.pause, GAMEOVER_);

  userInput(Terminate, true);
  game->finite_state_machine();
  EXPECT_EQ(game->state, EXIT_STATE);
}
