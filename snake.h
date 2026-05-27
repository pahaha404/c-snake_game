/**
 * @file snake.h
 * @brief Snake 객체의 생명 주기 및 이동 로직 선언 헤더
 * @details 뱀의 생성, 입력 키 검출, 충돌 확인(아이템, 게이트, 벽 등),
 *          그리고 이동 처리 멤버 함수를 정의합니다.
 * @author Hansol
 * @date 2026-05-28
 */
#pragma once

#include <ncurses.h>
#include <vector>
#include <time.h>
#include "board.h"

using namespace std;

struct snakepart {
    int x, y;
    snakepart(int col, int row);
    snakepart();
};

inline snakepart::snakepart(int col, int row) { x = col; y = row; }
inline snakepart::snakepart()                 { x = 0;   y = 0;   }

static const int head_way[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

class SnakeGame;   // 전방 선언 — game.h 순환 방지

class Snake {
public:
    int Head_Direction;
    int Body_length;
    int Growth_item;
    int Poison_item;
    int Reverse_active;
    time_t Reverse_start;
    int key_to_dir[4];
    vector<snakepart> snake;

    SnakeGame* game;

    Snake();
    void bind(SnakeGame* g) { game = g; }

    void make_snake();
    int  set_Head_Direction();
    int  move_Snake();
};

