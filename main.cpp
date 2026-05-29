#include "game.h"

int main() {
  setlocale(LC_ALL, "");

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  // SnakeGame 객체 생성
  SnakeGame snakeGame;

  if (has_colors()) {
    start_color();
    snakeGame.color_init();
  }

  snakeGame.win1 = newwin(30, 30, 1, 2);
  snakeGame.win2 = newwin(12, 23, 1, 35);
  snakeGame.win3 = newwin(13, 23, 14, 35);

  refresh();
  snakeGame.game();
  snakeGame.del_win();

  return 0;
}
