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

// 뱀의 구조체
struct snakepart {
    int x, y;
    snakepart(int col, int row);
    snakepart();
};

inline snakepart::snakepart(int col, int row) {
    x = col;
    y = row;
}

inline snakepart::snakepart() {
    x = 0;
    y = 0;
}

// 뱀의 이동 방향 가중치 상수 (정적 상수)
static const int head_way[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

// SnakeGame 클래스 정의
class SnakeGame {
public:
    // 1. 화면 출력용 ncurses 윈도우 객체
    WINDOW *win1, *win2, *win3;

    // 2. 게임 상태 변수들
    int Head_Direction;
    int Body_length;
    int Growth_item;
    int Poison_item;
    int Gate_cnt;
    int gate_posX;
    int gate_posY;
    int stage_num;
    int stage_flag;
    int current_speed_level;

    // 3. 방향 반전 아이템 관련 변수들
    int Reverse_active;
    time_t Reverse_start;
    int key_to_dir[4];

    // 4. 스네이크 및 게이트 좌표 저장 벡터들
    vector<snakepart> snake;
    vector<snakepart> gate;
    vector<snakepart> blue_gate;

    // 5. 생성자 (모든 멤버 변수 초기화)
    SnakeGame() {
        win1 = nullptr; 
        win2 = nullptr; 
        win3 = nullptr;
        Head_Direction = 1; 
        Body_length = 0; 
        Growth_item = 0; 
        Poison_item = 0; 
        Gate_cnt = 0; 
        gate_posX = 0; 
        gate_posY = 0;
        stage_num = 0; 
        stage_flag = 0;
        current_speed_level = 1;
        Reverse_active = 0;
        Reverse_start = 0;
        key_to_dir[0] = 0; 
        key_to_dir[1] = 1; 
        key_to_dir[2] = 2; 
        key_to_dir[3] = 3;
    }

    // 6. 게임 동작을 담당할 멤버 함수들 선언
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
    int pass_the_blue_gate();
    int finish_active_gate();
    void NEXTGAME(int num);
    void game();
};
