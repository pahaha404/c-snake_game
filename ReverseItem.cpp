// =============================================================================
// ReverseItem.cpp
// -----------------------------------------------------------------------------
// Reverse Direction Item 생성 로직 구현.
// 1~2 스테이지에선 등장하지 않게 하여 초반 학습 단계의 혼란을 막고,
// 3~4 스테이지부터 등장시켜 후반 난이도 상승 요소로 활용한다.
// =============================================================================
#include "ReverseItem.h"
#include "board.h"
#include <cstdlib>

void ReverseItem::generate(int stage_num) {
    // stage_num 은 0-기반이므로 "3 스테이지부터" 는 >= 2 조건으로 표현된다.
    if (stage_num < 2) return;

    int rx, ry;
    do {
        rx = rand() % 30;
        ry = rand() % 30;
    } while (map[stage_num][ry][rx] != 0);

    map[stage_num][ry][rx] = 8;
}
