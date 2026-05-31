// =============================================================================
// GoldenApple.cpp
// -----------------------------------------------------------------------------
// Golden Apple 생성 로직 구현.
// 다른 아이템과 달리 등장 자체에 확률 게이트가 있어, 게이트를 통과한 사이클
// 에서만 빈칸에 셀 값 9 를 둔다.
// =============================================================================
#include "GoldenApple.h"
#include "board.h"
#include <cstdlib>

void GoldenApple::generate(const int stage_num) {
    // 확률 게이트: rand() % N == 0 인 경우에만 통과.
    // N=10 → 사이클당 1/10 등장. N 값을 키우면 더 희귀해진다 (예: 20 → 1/20).
    if (rand() % 10 != 0) return;

    int gx, gy;
    do {
        gx = rand() % 30;
        gy = rand() % 30;
    } while (map[stage_num][gy][gx] != 0);

    map[stage_num][gy][gx] = 9;
}
