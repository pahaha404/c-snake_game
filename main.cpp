/**
 * @file main.cpp
 * @brief Snake Game의 진입점(Entry Point) 및 메인 루프 실행 파일
 * @details NCURSES 라이브러리를 초기화하고 SnakeGame 객체를 생성하여
 *          게임 화면(윈도우) 배치 및 실행을 관리합니다.
 * @author Hansol
 * @date 2026-05-28
 */
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
