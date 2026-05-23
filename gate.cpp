#include "game.h"
#include "gate.h"

// 노란 Gate, 파란 Gate 인스턴스 (파일 내부 정적 객체)
static Gate g_yellow(CELL_GATE);
static Gate g_blue(CELL_BLUE_GATE);

// 현재 통과 중인 gate를 가리키는 포인터 (1=노란, 2=파란, 0=통과 중 없음)
//   collision_gate() 에서 진입 시 설정
//   finish_active_gate() 에서 통과 완료 시 0으로 리셋
static int g_active_color = 0;

// =============================================================================
// Gate 클래스 구현
// =============================================================================

Gate::Gate(int cellVal)
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
    if (ex < 0) return inDir;  // 안전장치

    // 1) 가장자리 게이트는 무조건 안쪽으로
    if (isEdge(ex, ey)) {
        return inwardDir(ex, ey);
    }

    // 2) 가운데 게이트: 우선순위대로 진출 방향 시도
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
    return inDir;  // 모든 방향이 막혀있는 극단적 경우
}

void Gate::finish(const int stage_num) {
    if (!active) return;
    map[stage_num][y1][x1] = CELL_WALL;
    map[stage_num][y2][x2] = CELL_WALL;
    active     = false;
    in_idx     = -1;
    use_count += 1;
}

// -----------------------------------------------------------------------------
// 내부 헬퍼 함수들
// -----------------------------------------------------------------------------

bool Gate::isEdge(const int x, const int y) const {
    return (x == 0 || x == MAP_N - 1 || y == 0 || y == MAP_N - 1);
}

// 가장자리에서 맵 안쪽을 향하는 방향
//   왼쪽 벽   (x==0)    -> 오른쪽
//   오른쪽 벽 (x==29)   -> 왼쪽
//   위쪽 벽   (y==0)    -> 아래
//   아래쪽 벽 (y==29)   -> 위
int Gate::inwardDir(const int x, const int y) const {
    if (x == 0)            return DIR_RIGHT;
    if (x == MAP_N - 1)    return DIR_LEFT;
    if (y == 0)            return DIR_DOWN;
    if (y == MAP_N - 1)    return DIR_UP;
    return DIR_RIGHT;
}

// 시계방향 90도: UP -> RIGHT -> DOWN -> LEFT -> UP
int Gate::clockwise(const int dir) {
    switch (dir) {
        case DIR_UP:    return DIR_RIGHT;
        case DIR_RIGHT: return DIR_DOWN;
        case DIR_DOWN:  return DIR_LEFT;
        case DIR_LEFT:  return DIR_UP;
    }
    return dir;
}

// 반시계방향 90도: UP -> LEFT -> DOWN -> RIGHT -> UP
int Gate::counterClockwise(const int dir) {
    switch (dir) {
        case DIR_UP:    return DIR_LEFT;
        case DIR_LEFT:  return DIR_DOWN;
        case DIR_DOWN:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_UP;
    }
    return dir;
}

// 역방향 (180도)
int Gate::opposite(const int dir) {
    switch (dir) {
        case DIR_UP:    return DIR_DOWN;
        case DIR_DOWN:  return DIR_UP;
        case DIR_LEFT:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_LEFT;
    }
    return dir;
}

// (x, y) 칸으로 진출 가능한가: 맵 안 & 벽/면역벽이 아닌 칸
bool Gate::canExitTo(const int x, const int y, const int stage_num) const {
    if (x < 0 || x >= MAP_N || y < 0 || y >= MAP_N) return false;
    const int c = map[stage_num][y][x];
    return (c != CELL_WALL && c != CELL_IMMUNE);
}


// =============================================================================
// 기존 절차적 API 호환 구현
// -----------------------------------------------------------------------------
// snake.cpp, game.cpp 가 기존처럼 호출할 수 있도록 함수 형태를 유지하고,
// 내부에서는 위 Gate 클래스 인스턴스에 위임한다.
// =============================================================================

// -----------------------------------------------------------------------------
// 충돌 판정: snake 머리의 다음 위치가 게이트인지 확인
// 반환값: 7 (노란 gate), 11 (파란 gate), 0 (게이트 아님)
// -----------------------------------------------------------------------------
int SnakeGame::collision_gate() {
    const int nextX = snake[0].x + head_way[Head_Direction][0];
    const int nextY = snake[0].y + head_way[Head_Direction][1];

    // 노란 게이트는 vector 가 비어있거나 active=false 인 경우 진입 불가
    if (g_yellow.active && gate.size() >= 2 && g_yellow.checkEntry(nextX, nextY)) {
        // 기존 코드 호환: vector<snakepart> gate 의 [2] 슬롯에 in 인덱스 기록
        if (gate.size() > 2) gate.pop_back();
        gate.push_back(snakepart(g_yellow.in_idx, 1 - g_yellow.in_idx));
        g_active_color = 1;  // 노란 gate 통과 중
        return CELL_GATE;    // 7
    }
    if (g_blue.active && blue_gate.size() >= 2 && g_blue.checkEntry(nextX, nextY)) {
        if (blue_gate.size() > 2) blue_gate.pop_back();
        blue_gate.push_back(snakepart(g_blue.in_idx, 1 - g_blue.in_idx));
        g_active_color = 2;  // 파란 gate 통과 중
        return CELL_BLUE_GATE;  // 11
    }
    return 0;
}

// -----------------------------------------------------------------------------
// 게이트 한 쌍씩 생성 (노란, 파란 각각)
// -----------------------------------------------------------------------------
void SnakeGame::generate_gate() {
    srand((unsigned) time(0));

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int bx1 = 0, by1 = 0, bx2 = 0, by2 = 0;

    // 노란 게이트 1
    do {
        x1 = rand() % MAP_N;
        y1 = rand() % MAP_N;
    } while (map[stage_num][y1][x1] != CELL_WALL);

    // 노란 게이트 2 (1과 다른 위치)
    do {
        x2 = rand() % MAP_N;
        y2 = rand() % MAP_N;
    } while (map[stage_num][y2][x2] != CELL_WALL ||
             (x1 == x2 && y1 == y2));

    // 파란 게이트 1 (노란과 겹치지 않게)
    do {
        bx1 = rand() % MAP_N;
        by1 = rand() % MAP_N;
    } while (map[stage_num][by1][bx1] != CELL_WALL ||
             (bx1 == x1 && by1 == y1) ||
             (bx1 == x2 && by1 == y2));

    // 파란 게이트 2 (노란 둘 + 파란 1과 겹치지 않게)
    do {
        bx2 = rand() % MAP_N;
        by2 = rand() % MAP_N;
    } while (map[stage_num][by2][bx2] != CELL_WALL ||
             (bx2 == x1 && by2 == y1) ||
             (bx2 == x2 && by2 == y2) ||
             (bx2 == bx1 && by2 == by1));

    // 맵에 표시
    map[stage_num][y1][x1]   = CELL_GATE;
    map[stage_num][y2][x2]   = CELL_GATE;
    map[stage_num][by1][bx1] = CELL_BLUE_GATE;
    map[stage_num][by2][bx2] = CELL_BLUE_GATE;

    // Gate 객체 갱신
    g_yellow.x1 = x1; g_yellow.y1 = y1;
    g_yellow.x2 = x2; g_yellow.y2 = y2;
    g_yellow.in_idx = -1;
    g_yellow.active = true;

    g_blue.x1 = bx1; g_blue.y1 = by1;
    g_blue.x2 = bx2; g_blue.y2 = by2;
    g_blue.in_idx = -1;
    g_blue.active = true;

    // 통과 중 표시 리셋
    g_active_color = 0;

    // 기존 vector 호환 (snake.cpp 의 gate[0], gate[1], blue_gate[0], blue_gate[1] 사용)
    gate.push_back(snakepart(x1, y1));
    gate.push_back(snakepart(x2, y2));
    blue_gate.push_back(snakepart(bx1, by1));
    blue_gate.push_back(snakepart(bx2, by2));

    refresh();
}

// -----------------------------------------------------------------------------
// 노란 gate 통과 후 진출 방향 결정
// -----------------------------------------------------------------------------
int SnakeGame::pass_the_gate() {
    Head_Direction = g_yellow.computeExitDirection(Head_Direction, stage_num);
    return Head_Direction;
}

// -----------------------------------------------------------------------------
// 파란 gate 통과 후 진출 방향 결정
// -----------------------------------------------------------------------------
int SnakeGame::pass_the_blue_gate() {
    Head_Direction = g_blue.computeExitDirection(Head_Direction, stage_num);
    return Head_Direction;
}

// -----------------------------------------------------------------------------
// 현재 통과 중인 gate(노란 or 파란)를 자동 판별하여 정리.
//   - 해당 gate 쌍의 맵 셀을 Wall(1)로 복원
//   - 해당 vector 비우기
//   - use_count 반영 -> Gate_cnt 증가
// 반환: 1=노란 통과 완료, 2=파란 통과 완료, 0=아무것도 안 함
// -----------------------------------------------------------------------------
int SnakeGame::finish_active_gate() {
    if (g_active_color == 1) {
        // 노란 gate 통과 완료
        if (g_yellow.active) {
            g_yellow.finish(stage_num);
        }
        gate.clear();
        Gate_cnt += 1;
        g_active_color = 0;
        return 1;
    }
    else if (g_active_color == 2) {
        // 파란 gate 통과 완료
        if (g_blue.active) {
            g_blue.finish(stage_num);
        }
        blue_gate.clear();
        Gate_cnt += 1;
        g_active_color = 0;
        return 2;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// 진출 좌표 getter (안전한 접근용)
// snake.cpp 에서 gate[gate[2].y].x 같은 위험한 접근 대신 사용
// -----------------------------------------------------------------------------
int get_yellow_exit_x() {
    return g_yellow.getExitX();
}
int get_yellow_exit_y() {
    return g_yellow.getExitY();
}
int get_blue_exit_x() {
    return g_blue.getExitX();
}
int get_blue_exit_y() {
    return g_blue.getExitY();
}
