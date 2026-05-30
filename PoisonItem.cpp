// =============================================================================
// PoisonItem.cpp
// -----------------------------------------------------------------------------
// Poison Item 생성 로직 구현.
// 빈칸이 나올 때까지 좌표를 다시 뽑아 셀 값 6 을 둔다.
// =============================================================================
#include "PoisonItem.h"
#include "board.h"
#include <cstdlib>

void PoisonItem::generate(int stage_num) {
    int px, py;
    do {
        px = rand() % 30;
        py = rand() % 30;
    } while (map[stage_num][py][px] != 0);

    map[stage_num][py][px] = 6;
}
