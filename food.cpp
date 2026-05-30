/**
 * @file food.cpp
 * @brief Growth Item(먹이) 관리를 위한 Food 클래스 구현 파일
 * @details 30x30 보드의 빈 공간을 자동으로 탐색하여 Growth Item(5번)을 스폰합니다.
 * @author Hansol
 * @date 2026-05-28
 */
#include "food.h"
#include "board.h"
#include <cstdlib>

Food::Food() : x(0), y(0) {}

void Food::spawn(const int stage_num) {
    do {
        x = rand() % 30;
        y = rand() % 30;
    } while (map[stage_num][y][x] != 0);
    map[stage_num][y][x] = 5; // Growth Item
}
