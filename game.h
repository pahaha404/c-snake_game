#pragma once

#include <ncurses.h>
#include <vector>
#include <locale.h>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <string>
#include "map.h"

using namespace std;

// 필요한 색 정의
#define COLOR_GRAY 8
#define COLOR_BLACK_A 9

WINDOW *win1, *win2, *win3;
void color_init();
void color();
void score();
void set_zero();
void del_win();
void make_snake();
int set_Head_Direction();
int move_Snake();
void generate_item();
int collision_gate();
void generate_gate();
int pass_the_gate();
void NEXTGAME(int num);
void game();
int head_way[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
int Head_Direction, Body_length, Growth_item, Poison_item, Gate_cnt, gate_posX, gate_posY;
int stage_num=0, stage_flag=0;
int current_speed_level = 1; // snake의 현재 속도레벨 표시, 후에 스코어보드에서 표시할 수 있도록 전역변수 설정

//추가 시작
int Reverse_active = 0;
time_t Reverse_start;
int key_to_dir[4] = { 0, 1, 2, 3 };  // KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN → 논리 방향
//추가 끝

struct snakepart{
  int x, y;
  snakepart(int col, int row);
  snakepart();
};
snakepart::snakepart(int col, int row){
  x=col;
  y=row;
}
snakepart::snakepart(){
  x=0;
  y=0;
}

vector<snakepart> snake;
vector<snakepart> gate;
