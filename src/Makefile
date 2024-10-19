SHELL = /bin/sh

CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra -g
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++17
LFLAGS = -lcheck -lsubunit -lrt -lpthread -lm
GFLAGS = -fprofile-arcs -ftest-coverage

TETRIS_EXE_NAME = tetris
SNAKE_EXE_NAME = snake

TETRIS_TEST_NAME = tetris_test
SNAKE_TEST_NAME = snake_test

LIB_TETRIS_NAME = tetris.a
LIB_SNAKE_NAME = snake.a

LIB_TETRIS_SRC = $(wildcard brick_game/tetris/*.c)
LIB_SNAKE_SRC = $(wildcard brick_game/snake/*.cpp)
TEST_TETRIS_SRC = $(wildcard test/tetris/*.c)
TEST_SNAKE_SRC = $(wildcard test/snake/*.cpp)

TEST_TETRIS_O = $(TEST_TETRIS_SRC:.c=.o)
LIB_TETRIS_O = $(LIB_TETRIS_SRC:.c=.o)

TEST_SNAKE_O = $(TEST_SNAKE_SRC:.cpp=.o)
LIB_SNAKE_O = $(LIB_SNAKE_SRC:.cpp=.o)

GCOV_NAME = gcov_tests.info
GAME = 0

all: clean install
.PHONY: all clean tetris.a snake.a install uninstall dvi dist test gcov_report style_check style_fix rebuild valgrind_test cppcheck_test

install:
	@mkdir -p install/tetris
	@mkdir -p install/snake

	@make -s tetris_console
	@mv $(TETRIS_EXE_NAME) install/tetris/
	@touch install/tetris/high_score.txt
	@make -s snake_console
	@mv $(SNAKE_EXE_NAME) install/snake/

	@make -s tetris_desktop
	@mv gui/desktop/tetris_desktop install/tetris/tetris_desktop

	@make -s snake_desktop
	@mv gui/desktop/snake_desktop install/snake/snake_desktop
	@cd gui/desktop && make -s clean
	@cd ../../

	@rm -rf *.o brick_game/tetris/*.o

tetris_console: $(LIB_TETRIS_NAME)
	@$(CC) $(CFLAGS) -DTETRIS -c ./gui/cli/*.c -L. -l:$(LIB_TETRIS_NAME)
	@$(CC) $(CFLAGS) -DTETRIS brick_game/*.c *.o -o $(TETRIS_EXE_NAME) $(LFLAGS) -L. -l:$(LIB_TETRIS_NAME) -lncurses

snake_console: $(LIB_SNAKE_NAME)
	@g++ $(CXXFLAGS) -DSNAKE ./gui/cli/*.c brick_game/*.c -o $(SNAKE_EXE_NAME) -lsubunit -L. -l:$(LIB_SNAKE_NAME) -lncurses

tetris_desktop: $(LIB_TETRIS_NAME)
	@cd gui/desktop && qmake DEFINES+=TETRIS && make clean && make

snake_desktop: $(LIB_SNAKE_NAME)
	@cd gui/desktop && qmake DEFINES+=SNAKE && make clean && make

uninstall:
	@rm -rf install

clean:
	@rm -rf *.o *.a *.gcno *.gcda *.info report tetris_test snake_test latex html BrickGame-2.0.tgz BrickGame-2.0/ high_score.txt

tetris.a: $(LIB_TETRIS_O)
	@ar rc $(LIB_TETRIS_NAME) $(LIB_TETRIS_O)
	@ranlib $(LIB_TETRIS_NAME)
	@rm -rf brick_game/tetris/*.o

snake.a: $(LIB_SNAKE_O)
	@ar rc $(LIB_SNAKE_NAME) $(LIB_SNAKE_O)
	@ranlib $(LIB_SNAKE_NAME)
	@rm -rf brick_game/snake/*.o

play: menu
	@$(eval GAME := $(shell read -p "" input && echo $$input))
	@if [ "$(GAME)" = 1 ]; then \
		cd install/snake; \
		./snake; \
		exit 0; \
	elif [ "$(GAME)" = 2 ]; then \
		cd install/tetris; \
		./tetris; \
		exit 0; \
	elif [ "$(GAME)" = 3 ]; then \
		cd install/snake; \
		./snake_desktop; \
		exit 0; \
	elif [ "$(GAME)" = 4 ]; then \
		cd install/tetris; \
		./tetris_desktop; \
		exit 0; \
	else \
		echo "Incorrect number"; \
	fi

dvi:
	@doxygen Doxyfile
	@cd latex && xelatex refman.tex && cd ../

xelatex_install:
	sudo apt-get install texlive-xetex
	sudo apt-get install texlive-lang-cyrillic

dist: uninstall install
	@mkdir -p BrickGame-2.0/snake/
	@mkdir -p BrickGame-2.0/tetris/

	@cp install/snake/snake BrickGame-2.0/snake/
	@cp install/snake/snake_desktop BrickGame-2.0/snake/

	@cp install/tetris/tetris BrickGame-2.0/tetris/
	@cp install/tetris/tetris_desktop BrickGame-2.0/tetris/

	tar cvzf BrickGame-2.0.tgz BrickGame-2.0/
	@rm -rf BrickGame-2.0/

test: $(TEST_TETRIS_O) $(LIB_TETRIS_NAME) $(TEST_SNAKE_O) $(LIB_SNAKE_NAME)
	@$(CC) $(CFLAGS) $(TEST_TETRIS_O) -o $(TETRIS_TEST_NAME) -L. -l:$(LIB_TETRIS_NAME) $(LFLAGS)
	@./$(TETRIS_TEST_NAME)

	@g++ $(CXXFLAGS) -DTEST $(TEST_SNAKE_O) -o $(SNAKE_TEST_NAME) -L. -l:$(LIB_SNAKE_NAME) -lgtest -pthread -lstdc++ -lsupc++ -lgcc
	@./$(SNAKE_TEST_NAME)
	@rm -rf *.o test/snake/test_snake.o test/tetris/test_tetris.o high_score.txt 

gcov_report_tetris: clean
	@$(CC) $(CFLAGS) -c $(LIB_TETRIS_SRC) --coverage
	@$(CC) $(CFLAGS) -c $(TEST_TETRIS_SRC)
	@$(CC) *.o -o $(TETRIS_TEST_NAME) $(LFLAGS) $(GFLAGS)
	@./$(TETRIS_TEST_NAME)
	lcov -t "gcov_tests" -o $(GCOV_NAME) -c -d .
	genhtml -o report $(GCOV_NAME)
	@rm -rf *.gcno *.gcda *.gcov $(GCOV_NAME) *.o

gcov_report_snake: clean
	@g++ $(CXXFLAGS) -c $(LIB_SNAKE_SRC) --coverage
	@g++ $(CXXFLAGS) -c $(TEST_SNAKE_SRC)
	@g++ *.o -o $(SNAKE_TEST_NAME) $(LFLAGS) $(GFLAGS) -lgtest -pthread -lstdc++ -lsupc++ -lgcc
	./$(SNAKE_TEST_NAME)
	@lcov -t "gcov_tests" -o $(GCOV_NAME) -c -d . \
		--exclude '/usr/include/c++/11/*' \
		--exclude '/usr/include/c++/11/bits/*' \
		--exclude '/usr/include/c++/11/ext/*' 
	genhtml -o report $(GCOV_NAME)
	@rm -rf *.gcno *.gcda *.gcov $(GCOV_NAME) *.o

style_check:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n brick_game/**/*.c brick_game/**/*.h brick_game/**/*.cpp brick_game/*.c gui/**/*.c gui/**/*.h gui/**/*.cpp test/**/*.c test/**/*.h test/**/*.cpp
	@rm -rf .clang-format

style_fix:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -i brick_game/**/*.c brick_game/**/*.h brick_game/**/*.cpp brick_game/*.c gui/**/*.c gui/**/*.h gui/**/*.cpp test/**/*.c test/**/*.h test/**/*.cpp
	@rm -rf .clang-format

rebuild: clean tetris.a snake.a uninstall install

valgrind_test: rebuild test
	@valgrind --tool=memcheck --leak-check=yes --show-leak-kinds=all --track-origins=yes -s ./tetris_test
	@valgrind --tool=memcheck --leak-check=yes --show-leak-kinds=all --track-origins=yes -s ./snake_test

menu:
	@echo "<<BrickGame>>\n\n1. Snake (console)\n2. Tetris (console)\n\n3. Snake (desktop)\n4. Tetris (desktop)\n\nEnter a number:"
