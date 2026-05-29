#include "GrowthItem.h"
#include "map.h"
#include <cstdlib>

// Growth Item 생성: 반드시 빈칸(0)에만 생성
void generate_growth_item(int stage_num) {
    int gx, gy;
    do {
        gx = rand() % 30;
        gy = rand() % 30;
    } while (map[stage_num][gy][gx] != 0);

    map[stage_num][gy][gx] = 5;
}