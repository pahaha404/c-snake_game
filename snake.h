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
inline snakepart::snakepart() { x = 0;   y = 0; }

static const int head_way[4][2] = { {0, -1}, {-1, 0}, {1, 0}, {0, 1} };

class SnakeGame;   // 전방 선언 — game.h 순환 방지

class Snake {
public:
    int Head_Direction;
    int Body_length;
    int Growth_item;
    int Poison_item;

    // Reverse Item 의 활성 상태와 시작 시각(time(NULL) 기준).
    // Reverse_active 가 켜져 있는 동안 set_Head_Direction() 이 매 틱
    // update_reverse_keys() 를 호출해 진행방향에 맞춰 key_to_dir 을 갱신한다.
    int    Reverse_active;
    time_t Reverse_start;

    int key_to_dir[4];
    vector<snakepart> snake;

    // Golden Apple 획득 시 남은 추가 성장 칸 수.
    // 일반 이동 분기에서 1 이상이면 꼬리를 자르지 않고 1 씩 차감한다.
    // 먹은 시점에 2 를 더하므로 (먹은 틱 머리 추가 +1) + (이후 2 틱) = +3.
    int pending_growth;

    SnakeGame* game;

    Snake();
// Snake가 게이트, 스테이지, 게임오버 화면처럼 SnakeGame의 기능을 호출할 수 있도록 현재 객체를 연결합니다.
    void bind(SnakeGame* g) { game = g; }

    void make_snake();
    int  set_Head_Direction();
    int  move_Snake();

    // Reverse Item 효과 적용: 현재 진행방향에 수직인 두 화살표 키의 기능을
    // 서로 맞바꾸어 key_to_dir 을 갱신한다.
    void update_reverse_keys();
};
