/**
 * @file game.h
 * @brief Snake Game의 메인 제어 클래스(SnakeGame) 정의 헤더
 * @details 전체 게임 루프 조율, NCURSES 윈도우 인스턴스 관리,
 *          점수 계산 및 최고 점수 로딩/저장 기능을 총괄합니다.
 * @author Hansol
 * @date 2026-05-28
 */
#pragma once

#include <ncurses.h>
#include <vector>
#include <locale.h>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <string>
#include <fstream>
#include "board.h"
#include "snake.h"      
#include "gate.h"       
#include "GrowthItem.h"
#include "PoisonItem.h"
#include "ReverseItem.h"
#include "GoldenApple.h"

using namespace std;

#define COLOR_GRAY 8
#define COLOR_BLACK_A 9

class SnakeGame {
public:
    WINDOW* win1, * win2, * win3;

    int Gate_cnt;
    int gate_posX;
    int gate_posY;
    int stage_num;
    int stage_flag;
    int current_speed_level; // 현재 스피드 레벨 저장용 변수
    int High_Score; // 최고 점수 저장용 변수

    vector<snakepart> gate;
    vector<snakepart> blue_gate;

    Snake snake_obj;

    Gate yellow_gate;       // 노란 게이트 (cell_value = 7)
    Gate blue_gate_obj;     // 파란 게이트 (cell_value = 11)

    int  active_gate_color; // 통과 중인 게이트: 1=노란, 2=파란, 0=없음

    bool visited[30][30];
    bool easter_egg_eligible;
    time_t stage_start_time;

    // alias: gate.cpp/item.cpp가 이전 이름으로 접근 가능
    int& Head_Direction = snake_obj.Head_Direction;
    int& Body_length = snake_obj.Body_length;
    int& Growth_item = snake_obj.Growth_item;
    int& Poison_item = snake_obj.Poison_item;
    int& Reverse_active = snake_obj.Reverse_active;
    time_t& Reverse_start = snake_obj.Reverse_start;
    int(&key_to_dir)[4] = snake_obj.key_to_dir;
    vector<snakepart>& snake = snake_obj.snake;

    SnakeGame()
        : yellow_gate(CELL_GATE),
        blue_gate_obj(CELL_BLUE_GATE),
        active_gate_color(0)
    {
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

    
    int  collision_gate();
    void generate_gate();
    int  pass_the_gate();
    int  pass_the_blue_gate();
    int  finish_active_gate();

    // 게이트 진출 좌표 getter
    int  get_yellow_exit_x() const;
    int  get_yellow_exit_y() const;
    int  get_blue_exit_x()   const;
    int  get_blue_exit_y()   const;

    void NEXTGAME(const int num);
    void game();
};
