#include "game.cpp"

int main() {
  setlocale(LC_ALL, "");

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  if (has_colors()) {
    start_color();
    color_init();
  }

  win1 = newwin(30, 30, 1, 2);
  win2 = newwin(12, 20, 1, 35);
  win3 = newwin(12, 20, 14, 35);

  refresh();
  game();
  del_win();

  return 0;
}
