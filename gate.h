// =============================================================================
// gate.h
// -----------------------------------------------------------------------------
// Snake Game의 Gate(출입구) 시스템을 정의하는 헤더 파일.
//
// 본 파일은 다음 요소를 포함한다:
//   - 방향 상수 (DIR_UP / DIR_LEFT / DIR_RIGHT / DIR_DOWN)
//     : game.h의 head_way[4][2] = {{0,-1},{-1,0},{1,0},{0,1}} 와 동일한 매핑
//   - 맵 셀 값 상수 (CELL_BLANK, CELL_WALL, CELL_IMMUNE, CELL_GATE, CELL_BLUE_GATE)
//   - 맵 크기 상수 (MAP_N)
//   - Gate 클래스 선언
//
// Gate 클래스는 한 쌍(2개)의 게이트를 묶어 관리한다. 노란 게이트와
// 파란 게이트는 SnakeGame이 각각 별도의 Gate 객체(멤버)로 보유한다.
// =============================================================================
#ifndef GATE_H
#define GATE_H

// =============================================================================
// 방향 상수
//   game.h 의 head_way[4][2] = {{0,-1},{-1,0},{1,0},{0,1}} 와 동일한 매핑.
//   UP=0, LEFT=1, RIGHT=2, DOWN=3
// =============================================================================
enum Direction {
    DIR_UP    = 0,
    DIR_LEFT  = 1,
    DIR_RIGHT = 2,
    DIR_DOWN  = 3
};

// 셀 값 상수: map 배열에서 사용하는 값들
const int CELL_BLANK     = 0;
const int CELL_WALL      = 1;
const int CELL_IMMUNE    = 2;
const int CELL_GATE      = 7;
const int CELL_BLUE_GATE = 11;

const int MAP_N = 30;  // 맵 크기 (30x30)

// =============================================================================
// Gate 클래스
// -----------------------------------------------------------------------------
// 한 쌍(2개)의 게이트를 묶어서 관리한다.
// 노란 게이트와 파란 게이트는 각각 별도의 Gate 객체로 운영한다.
// =============================================================================
class Gate {
public:
    int x1, y1;
    int x2, y2;

    int in_idx;       // 진입한 게이트 인덱스 (0=첫번째, 1=두번째, -1=미진입)
    int use_count;    // 사용 횟수
    bool active;      // 활성 상태 (generate 후 ~ clear 전까지 true)
    int cell_value;   // 이 Gate의 맵 셀 값 (7 또는 11)

    Gate(const int cellVal = CELL_GATE);

    // 진입 판정: 다음 이동 위치가 게이트와 일치하면 in_idx 기록 후 true
    bool checkEntry(const int nextX, const int nextY);

    // 진출 게이트의 좌표
    int getExitX() const;
    int getExitY() const;

    // 진출 방향 계산 (명세 우선순위: 진입→시계→반시계→역방향)
    int computeExitDirection(const int inDir, const int stage_num) const;

    // 게이트 한 쌍 통과 완료: 셀을 Wall(1)로 복원, use_count +1, 비활성화
    void finish(const int stage_num);

private:
    bool isEdge(const int x, const int y) const;
    int  inwardDir(const int x, const int y) const;
    static int clockwise(const int dir);
    static int counterClockwise(const int dir);
    static int opposite(const int dir);
    bool canExitTo(const int x, const int y, const int stage_num) const;
};

#endif