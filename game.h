#pragma once

#include <ncurses.h>
#include <vector>
#include <locale.h>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <string>
#include <fstream>
#include "map.h"
#include "snake.h"      
#include "gate.h"       

using namespace std;

#define COLOR_GRAY 8
#define COLOR_BLACK_A 9

// snakepart, head_way 는 snake.h로 이동했으므로 여기서 제거

class SnakeGame {
public:
    WINDOW *win1, *win2, *win3;

    int Gate_cnt;
    int gate_posX;
    int gate_posY;
    int stage_num;
    int stage_flag;
    int current_speed_level;
    int High_Score; // 최고 점수 저장용 변수

    vector<snakepart> gate;
    vector<snakepart> blue_gate;

    Snake snake_obj;

    // alias: gate.cpp/item.cpp가 이전 이름으로 접근 가능
    int&               Head_Direction = snake_obj.Head_Direction;
    int&               Body_length    = snake_obj.Body_length;
    int&               Growth_item    = snake_obj.Growth_item;
    int&               Poison_item    = snake_obj.Poison_item;
    int&               Reverse_active = snake_obj.Reverse_active;
    time_t&            Reverse_start  = snake_obj.Reverse_start;
    int              (&key_to_dir)[4] = snake_obj.key_to_dir;
    vector<snakepart>& snake          = snake_obj.snake;

    SnakeGame() {
        win1 = nullptr; win2 = nullptr; win3 = nullptr;
        Gate_cnt = 0;
        gate_posX = 0; gate_posY = 0;
        stage_num = 0; stage_flag = 0;
        current_speed_level = 1;

        // 파일로부터 최고 점수 불러오기
        High_Score = 0;
        ifstream infile("highscore.txt");
        if (infile.is_open()) {
            infile >> High_Score;
            infile.close();
        }

        snake_obj.bind(this);
    }

    void color_init();
    void color();
    void score();
    void set_zero();
    void del_win();

    // 스네이크 3종은 Snake로 이전 → 선언 제거
    // void make_snake();
    // int  set_Head_Direction();
    // int  move_Snake();

    // 아이템 (그대로 유지)
    void generate_item();

    
    int  collision_gate();
    void generate_gate();
    int  pass_the_gate();
    int  pass_the_blue_gate();
    int  finish_active_gate();

    void NEXTGAME(const int num);
    void game();
};

