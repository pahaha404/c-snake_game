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
    // 두 게이트의 좌표
    int x1, y1;
    int x2, y2;

    // 진입한 게이트의 인덱스 (0=첫번째, 1=두번째, -1=아직 진입 안 함)
    int in_idx;

    // 사용 횟수 (외부에서 Gate_cnt 갱신에 사용)
    int use_count;

    // 활성 상태 (generate 후 ~ clear 전까지 true)
    bool active;

    // 이 Gate의 맵 셀 값 (7 또는 11)
    int cell_value;

    Gate(int cellVal = CELL_GATE);

    // 진입 판정: 다음 이동 위치가 게이트와 일치하면 in_idx 기록 후 true
    bool checkEntry(const int nextX, const int nextY);

    // 진출 게이트의 x 좌표
    int getExitX() const;

    // 진출 게이트의 y 좌표
    int getExitY() const;

    // 진출 방향 계산 (명세 우선순위: 진입→시계→반시계→역방향)
    //   inDir : Snake가 게이트로 진입할 당시의 진행 방향
    int computeExitDirection(const int inDir, const int stage_num) const;

    // 게이트 한 쌍 통과 완료: 셀을 Wall(1)로 복원, use_count +1, 비활성화
    void finish(const int stage_num);

private:
    // 가장자리(테두리) 여부
    bool isEdge(const int x, const int y) const;

    // 가장자리에서 안쪽 방향
    int  inwardDir(const int x, const int y) const;

    // 시계방향/반시계방향/역방향 회전
    static int clockwise(const int dir);
    static int counterClockwise(const int dir);
    static int opposite(const int dir);

    // (x, y) 칸으로 진출 가능한지: Wall(1) 또는 ImmuneWall(2)이 아닌 칸
    bool canExitTo(const int x, const int y, const int stage_num) const;
};

// 일반 gate 충돌 확인
int collision_gate();

// gate 생성 (노란 + 파란 한 쌍씩)
void generate_gate();

// 노란 gate 통과 후 진출 방향 결정
int pass_the_gate();

// 파란 gate 통과 후 진출 방향 결정
int pass_the_blue_gate();

// 통과 중인 gate가 없으면 아무것도 하지 않음.
// 반환: 1이면 노란 gate 통과 완료, 2면 파란 gate 통과 완료, 0이면 아무것도 안함
int finish_active_gate();

// -----------------------------------------------------------------------------
// 노란 gate의 진출 좌표 반환 (안전한 접근용)
//   gate[gate[2].y].x 같은 위험한 vector 접근 대신 사용
// 반환: -1이면 진입 정보 없음
// -----------------------------------------------------------------------------
int get_yellow_exit_x();
int get_yellow_exit_y();

// 파란 gate의 진출 좌표 반환
int get_blue_exit_x();
int get_blue_exit_y();

#endif