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

using namespace std;

#define COLOR_GRAY 8
#define COLOR_BLACK_A 9

class SnakeGame {
public:
    WINDOW *win1, *win2, *win3;

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

    // Snake 객체가 실제로 소유한 상태를 SnakeGame에서도 기존 멤버처럼 접근하기 위한 참조 alias입니다.
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

        // Snake가 게이트, 스테이지, 게임오버 화면처럼 SnakeGame의 기능을 호출할 수 있도록 현재 객체를 연결합니다.
        snake_obj.bind(this);
    }

    void color_init();
    void color();
    void score();
    void set_zero();
    void del_win();

    void generate_item();

    
    int  collision_gate();
    void generate_gate();
    int  pass_the_gate();
    int  pass_the_blue_gate();
    int  finish_active_gate();

    void NEXTGAME(const int num);
    void game();
};

