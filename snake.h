/**
 * 객체의 상태와 이동 로직을 선언하는 헤더 파일
 * 뱀의 몸통 좌표, 현재 진행 방향, 아이템 획득 수, Reverse 효과 상태를 관리하는
 * Snake 클래스와 좌표 단위 구조체를 정의합니다. 입력 처리, 충돌 판정, 아이템 및
 * 게이트 통과 처리는 snake.cpp에서 구현됩니다.
 */

#pragma once

#include <ncurses.h>
#include <vector>
#include <time.h>
#include "board.h"

using namespace std;

/**
 * 뱀 몸통 한 칸의 보드 좌표를 저장하는 구조체
 * x는 열(col), y는 행(row)을 의미합니다.
 */
struct snakepart {
    int x, y;
    snakepart(int col, int row);
    snakepart();
};

inline snakepart::snakepart(int col, int row) { x = col; y = row; }
inline snakepart::snakepart()                 { x = 0;   y = 0;   }

// 방향 인덱스는 0: 위, 1: 왼쪽, 2: 오른쪽, 3: 아래를 의미합니다.
static const int head_way[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

class SnakeGame;   // 전방 선언으로 game.h와의 순환 참조를 방지합니다.

/**
 * 뱀의 현재 상태와 한 턴 단위 이동을 담당하는 클래스
 * Snake는 몸통 좌표 벡터를 직접 관리하고, 보드 map에는 머리(3), 몸통(4) 등
 * 렌더링에 필요한 값을 기록합니다. 스테이지 번호, 게이트 처리, 게임 종료 화면처럼
 * 전체 게임 흐름이 필요한 작업은 bind된 SnakeGame 객체에 위임합니다.
 */
class Snake {
public:
    // 진행 방향과 미션 점수 상태
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

    // Snake가 SnakeGame의 스테이지/게이트/화면 제어 기능을 사용할 수 있도록 연결합니다.
    void bind(SnakeGame* g) { game = g; }

    // 스테이지 시작 시 초기 뱀 위치를 생성하고 보드 map에 반영합니다.
    void make_snake();

    // 키 입력을 현재 방향으로 변환하고, 즉시 반대 방향 전환 시 게임 오버를 처리합니다.
    int  set_Head_Direction();

    // 현재 방향으로 한 칸 이동하며 충돌, 아이템, 게이트 효과를 처리합니다.
    int  move_Snake();
};
