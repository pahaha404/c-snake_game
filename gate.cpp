// =============================================================================
// gate.cpp
// -----------------------------------------------------------------------------
// Snake Game의 Gate(출입구) 시스템 구현 파일.
//
// 본 파일은 두 부분으로 구성된다:
//
//   1) Gate 클래스의 멤버 함수 구현
//        - 게이트 진입/진출 좌표 계산
//        - 진출 방향 결정 알고리즘
//          (명세 우선순위: 진입 → 시계 → 반시계 → 역방향)
//        - 게이트 통과 완료 처리 (맵 셀 복원)
//
//   2) SnakeGame 클래스의 게이트 관련 멤버 함수 구현
//        - collision_gate()      : Snake 머리의 다음 칸이 게이트인지 판정
//        - generate_gate()       : 노란/파란 게이트 한 쌍씩 무작위 배치
//        - pass_the_gate()       : 노란 gate 통과 후 진출 방향 갱신
//        - pass_the_blue_gate()  : 파란 gate 통과 후 진출 방향 갱신
//        - finish_active_gate()  : 통과 완료 후 게이트 정리
//        - get_yellow_exit_x/y() : 노란 게이트의 진출 좌표 getter
//        - get_blue_exit_x/y()   : 파란 게이트의 진출 좌표 getter
//
// 전역 변수를 사용하지 않기 위해, 두 개의 Gate 인스턴스(yellow_gate,
// blue_gate_obj)와 현재 통과 중인 게이트 색을 나타내는 active_gate_color는
// SnakeGame 클래스의 멤버로 보관한다.
// =============================================================================
#include "game.h"
#include "gate.h"

// =============================================================================
// Gate 클래스 구현
// =============================================================================

Gate::Gate(const int cellVal)
    : x1(0), y1(0), x2(0), y2(0),
      in_idx(-1), use_count(0), active(false), cell_value(cellVal)
{
}

bool Gate::checkEntry(const int nextX, const int nextY) {
    if (!active) return false;
    if (nextX == x1 && nextY == y1) { in_idx = 0; return true; }
    if (nextX == x2 && nextY == y2) { in_idx = 1; return true; }
    return false;
}

int Gate::getExitX() const {
    if (in_idx == 0) return x2;
    if (in_idx == 1) return x1;
    return -1;
}

int Gate::getExitY() const {
    if (in_idx == 0) return y2;
    if (in_idx == 1) return y1;
    return -1;
}

// -----------------------------------------------------------------------------
// 진출 방향 계산: 명세 우선순위(진입 → 시계 → 반시계 → 역방향)
// -----------------------------------------------------------------------------
int Gate::computeExitDirection(const int inDir, const int stage_num) const {
    const int ex = getExitX();
    const int ey = getExitY();
    if (ex < 0) return inDir;

    if (isEdge(ex, ey)) {
        return inwardDir(ex, ey);
    }

    const int candidates[4] = {
        inDir,
        clockwise(inDir),
        counterClockwise(inDir),
        opposite(inDir)
    };
    for (int i = 0; i < 4; ++i) {
        const int d  = candidates[i];
        const int nx = ex + head_way[d][0];
        const int ny = ey + head_way[d][1];
        if (canExitTo(nx, ny, stage_num)) {
            return d;
        }
    }
    return inDir;
}

void Gate::finish(const int stage_num) {
    if (!active) return;
    map[stage_num][y1][x1] = CELL_WALL;
    map[stage_num][y2][x2] = CELL_WALL;
    active     = false;
    in_idx     = -1;
    use_count += 1;
}

bool Gate::isEdge(const int x, const int y) const {
    return (x == 0 || x == MAP_N - 1 || y == 0 || y == MAP_N - 1);
}

int Gate::inwardDir(const int x, const int y) const {
    if (x == 0)            return DIR_RIGHT;
    if (x == MAP_N - 1)    return DIR_LEFT;
    if (y == 0)            return DIR_DOWN;
    if (y == MAP_N - 1)    return DIR_UP;
    return DIR_RIGHT;
}

int Gate::clockwise(const int dir) {
    switch (dir) {
        case DIR_UP:    return DIR_RIGHT;
        case DIR_RIGHT: return DIR_DOWN;
        case DIR_DOWN:  return DIR_LEFT;
        case DIR_LEFT:  return DIR_UP;
    }
    return dir;
}

int Gate::counterClockwise(const int dir) {
    switch (dir) {
        case DIR_UP:    return DIR_LEFT;
        case DIR_LEFT:  return DIR_DOWN;
        case DIR_DOWN:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_UP;
    }
    return dir;
}

int Gate::opposite(const int dir) {
    switch (dir) {
        case DIR_UP:    return DIR_DOWN;
        case DIR_DOWN:  return DIR_UP;
        case DIR_LEFT:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_LEFT;
    }
    return dir;
}

bool Gate::canExitTo(const int x, const int y, const int stage_num) const {
    if (x < 0 || x >= MAP_N || y < 0 || y >= MAP_N) return false;
    const int c = map[stage_num][y][x];
    return (c != CELL_WALL && c != CELL_IMMUNE);
}


// =============================================================================
// SnakeGame의 게이트 관련 멤버 함수 구현
// -----------------------------------------------------------------------------
// 두 개의 Gate 인스턴스(yellow_gate, blue_gate_obj)와 현재 통과 중인
// 게이트 색(active_gate_color)은 모두 SnakeGame의 멤버로 보관한다.
// (파일 정적 변수를 사용하지 않기 위함)
// =============================================================================

int SnakeGame::collision_gate() {
    const int nextX = snake[0].x + head_way[Head_Direction][0];
    const int nextY = snake[0].y + head_way[Head_Direction][1];

    if (yellow_gate.active && gate.size() >= 2 &&
        yellow_gate.checkEntry(nextX, nextY)) {
        if (gate.size() > 2) gate.pop_back();
        gate.push_back(snakepart(yellow_gate.in_idx, 1 - yellow_gate.in_idx));
        active_gate_color = 1;
        return CELL_GATE;
    }
    if (blue_gate_obj.active && blue_gate.size() >= 2 &&
        blue_gate_obj.checkEntry(nextX, nextY)) {
        if (blue_gate.size() > 2) blue_gate.pop_back();
        blue_gate.push_back(snakepart(blue_gate_obj.in_idx,
                                      1 - blue_gate_obj.in_idx));
        active_gate_color = 2;
        return CELL_BLUE_GATE;
    }
    return 0;
}

void SnakeGame::generate_gate() {
    srand((unsigned) time(0));

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int bx1 = 0, by1 = 0, bx2 = 0, by2 = 0;

    do {
        x1 = rand() % MAP_N;
        y1 = rand() % MAP_N;
    } while (map[stage_num][y1][x1] != CELL_WALL);

    do {
        x2 = rand() % MAP_N;
        y2 = rand() % MAP_N;
    } while (map[stage_num][y2][x2] != CELL_WALL ||
             (x1 == x2 && y1 == y2));

    do {
        bx1 = rand() % MAP_N;
        by1 = rand() % MAP_N;
    } while (map[stage_num][by1][bx1] != CELL_WALL ||
             (bx1 == x1 && by1 == y1) ||
             (bx1 == x2 && by1 == y2));

    do {
        bx2 = rand() % MAP_N;
        by2 = rand() % MAP_N;
    } while (map[stage_num][by2][bx2] != CELL_WALL ||
             (bx2 == x1 && by2 == y1) ||
             (bx2 == x2 && by2 == y2) ||
             (bx2 == bx1 && by2 == by1));

    map[stage_num][y1][x1]   = CELL_GATE;
    map[stage_num][y2][x2]   = CELL_GATE;
    map[stage_num][by1][bx1] = CELL_BLUE_GATE;
    map[stage_num][by2][bx2] = CELL_BLUE_GATE;

    yellow_gate.x1 = x1; yellow_gate.y1 = y1;
    yellow_gate.x2 = x2; yellow_gate.y2 = y2;
    yellow_gate.in_idx = -1;
    yellow_gate.active = true;

    blue_gate_obj.x1 = bx1; blue_gate_obj.y1 = by1;
    blue_gate_obj.x2 = bx2; blue_gate_obj.y2 = by2;
    blue_gate_obj.in_idx = -1;
    blue_gate_obj.active = true;

    active_gate_color = 0;

    gate.push_back(snakepart(x1, y1));
    gate.push_back(snakepart(x2, y2));
    blue_gate.push_back(snakepart(bx1, by1));
    blue_gate.push_back(snakepart(bx2, by2));

    refresh();
}

int SnakeGame::pass_the_gate() {
    Head_Direction = yellow_gate.computeExitDirection(Head_Direction, stage_num);
    return Head_Direction;
}

int SnakeGame::pass_the_blue_gate() {
    Head_Direction = blue_gate_obj.computeExitDirection(Head_Direction, stage_num);
    return Head_Direction;
}

int SnakeGame::finish_active_gate() {
    if (active_gate_color == 1) {
        if (yellow_gate.active) {
            yellow_gate.finish(stage_num);
        }
        gate.clear();
        Gate_cnt += 1;
        active_gate_color = 0;
        return 1;
    }
    else if (active_gate_color == 2) {
        if (blue_gate_obj.active) {
            blue_gate_obj.finish(stage_num);
        }
        blue_gate.clear();
        Gate_cnt += 1;
        active_gate_color = 0;
        return 2;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// 진출 좌표 getter (안전한 접근용)
// -----------------------------------------------------------------------------
int SnakeGame::get_yellow_exit_x() const { return yellow_gate.getExitX(); }
int SnakeGame::get_yellow_exit_y() const { return yellow_gate.getExitY(); }
int SnakeGame::get_blue_exit_x()   const { return blue_gate_obj.getExitX(); }
int SnakeGame::get_blue_exit_y()   const { return blue_gate_obj.getExitY(); }