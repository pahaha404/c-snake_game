#pragma once

#include <ncurses.h>
#include <vector>
#include <time.h>
#include "map.h"

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

