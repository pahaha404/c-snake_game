#include "game.h"

void SnakeGame::color_init(){
  // 글씨 배경 순서
  init_color(COLOR_GRAY, 150, 150, 150);      // 회색으로 설정
  init_pair(1, COLOR_GRAY, COLOR_GRAY);       // Wall
  init_color(COLOR_BLACK_A, 0, 0, 0);         // 검은색으로 설정
  init_pair(2, COLOR_BLACK_A, COLOR_BLACK_A); // Immune Wall
  init_pair(3, COLOR_CYAN, COLOR_CYAN);       // Snake Head
  init_pair(4, COLOR_BLUE, COLOR_BLUE);       // Snake Body
  init_pair(5, COLOR_GREEN, COLOR_GREEN);     // Growth Item
  init_pair(6, COLOR_RED, COLOR_RED);         // Poison Item
  init_pair(7, COLOR_YELLOW, COLOR_YELLOW);   // Gate
  init_pair(8, COLOR_WHITE, COLOR_WHITE);     // Blank
  // ── 추가 ──
  init_pair(9, COLOR_MAGENTA, COLOR_MAGENTA); // Reverse Direction Item
  init_pair(11, COLOR_BLUE, COLOR_BLUE); // blue gate 추가
  //추가 끝
}

void SnakeGame::color(){
  // 기존의 반복적인 대형 if-else 구문을 배열 매핑으로 최적화 및 간소화
  int pair_map[] = {8, 1, 2, 3, 4, 5, 6, 7, 9};
  for(int i=0; i<30; i++){
    for(int j=0; j<30; j++){
      int val = map[stage_num][i][j];
      if(val >= 0 && val <= 8){
        wattron(win1, COLOR_PAIR(pair_map[val]));
        // 기존 %d(숫자) 출력에서 공백(" ") 출력으로 변경하여 색상 블록만 렌더링
        mvwprintw(win1, i, j, " ");
        wattroff(win1, COLOR_PAIR(pair_map[val]));
      }
      else if(val == 11){
        wattron(win1, COLOR_PAIR(11));
        mvwprintw(win1, i, j, " ");
        wattroff(win1, COLOR_PAIR(11));
      }//추가
    }
  }
  wrefresh(win1);
}

void SnakeGame::score(){
  // Body_length, Growth_item, Poison_item, Gate_cnt
  int goal[4][4] = {{5, 2, 1, 1}, {10, 4, 2, 2}, {15, 6, 4, 3}, {15, 6, 5, 4}};
  // 목표 점수 표시
  wborder(win2, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(win2, 1, 6, "*Mission*");
  mvwprintw(win2, 3, 8, "B : %d", goal[stage_num][0]);
  mvwprintw(win2, 5, 8, "+ : %d", goal[stage_num][1]);
  mvwprintw(win2, 7, 8, "- : %d", goal[stage_num][2]);
  mvwprintw(win2, 9, 8, "G : %d", goal[stage_num][3]);
  mvwprintw(win3, 11, 2, "SPEED :    %d", current_speed_level); // 스코어보드에 speed 추가

  int size = snake.size();
  int current_size = snake.size();
  Body_length = max(size, Body_length); // max_size

  // 점수 표시
  wborder(win3, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(win3, 1, 7, "*Score*");
  mvwprintw(win3, 3, 2, "B : %d / %d", current_size, Body_length);  // current / max
  mvwprintw(win3, 5, 2, "+ :   %d   ", Growth_item);
  mvwprintw(win3, 7, 2, "- :   %d   ", Poison_item);
  mvwprintw(win3, 9, 2, "G :   %d   ", Gate_cnt);

  // Body_length, 즉 stage 내 max_size가 목표 점수보다 크면 V 표시, 아니면 빈 칸
  if(Body_length < goal[stage_num][0])  mvwprintw(win3, 3, 14, "(   )");
  else  mvwprintw(win3, 3, 14, "( V )");

  // 한 stage에서 목표 점수만큼 아이템을 먹었을 경우 V 표시, 아니면 빈 칸
  if(Growth_item < goal[stage_num][1])  mvwprintw(win3, 5, 14, "(   )");
  else  mvwprintw(win3, 5, 14, "( V )");

  // 한 stage에서 목표 점수만큼 아이템을 먹었을 경우 V 표시, 아니면 빈 칸
  if(Poison_item < goal[stage_num][2])  mvwprintw(win3, 7, 14, "(   )");
  else  mvwprintw(win3, 7, 14, "( V )");

  // 한 stage에서 목표 점수만큼 gate를 지났을 경우 V 표시, 아니면 빈 칸
  if(Gate_cnt < goal[stage_num][3]) mvwprintw(win3, 9, 14, "(   )");
  else  mvwprintw(win3, 9, 14, "( V )");

  if((Body_length >= goal[stage_num][0]) && (Growth_item >= goal[stage_num][1]) &&
     (Poison_item >= goal[stage_num][2]) && (Gate_cnt >= goal[stage_num][3])){
       stage_num++;
       stage_flag = 1;
  }

  wrefresh(win2);
  wrefresh(win3);
}

void SnakeGame::set_zero(){
  stage_flag = 0;
  Body_length = 0;
  Growth_item = 0;
  Poison_item = 0;
  Gate_cnt = 0;
  gate_posX = 0;
  gate_posY = 0;
  // ── 추가 ──
  Reverse_active = 0;
  key_to_dir[0] = 0; key_to_dir[1] = 1;
  key_to_dir[2] = 2; key_to_dir[3] = 3;
  //추가 끝
}

void SnakeGame::NEXTGAME(int num){
  wclear(win1);
  if(num == 1){
    mvwprintw(win1, 10, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
    mvwprintw(win1, 11, 24, "★      STAGE CLEAR      ★");
    mvwprintw(win1, 12, 24, "★        ٩( ᐛ )و        ★");
    mvwprintw(win1, 13, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
  }
  else if(num == 2){
    mvwprintw(win1, 10, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
    mvwprintw(win1, 11, 24, "★     STAGE ALL CLEAR   ★");
    mvwprintw(win1, 12, 24, "★       ( ღ 'ᴗ'ღ )      ★");
    mvwprintw(win1, 13, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
  }
  else if(num == 3){
    mvwprintw(win1, 10, 28, "+ - - - - - - - - - - - +");
    mvwprintw(win1, 11, 28, "|    G A M E O V E R    |");
    mvwprintw(win1, 12, 28, "|        ( ಥ﹏ಥ)        |");
    mvwprintw(win1, 13, 28, "+ - - - - - - - - - - - +");
  }
  wrefresh(win1);
  nodelay(stdscr, false);
  getch();
  wclear(win1);
}

void SnakeGame::del_win(){
  delwin(win3);
  delwin(win2);
  delwin(win1);
  endwin();
}
