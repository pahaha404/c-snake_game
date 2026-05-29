#include "ReverseItem.h"
#include "map.h"
#include <cstdlib>

// Reverse Direction Item 생성: 반드시 빈칸(0)에만 생성
//   스테이지 3(내부 stage_num == 2)부터만 등장한다.
//   stage_num: 0=1스테이지, 1=2스테이지, 2=3스테이지, 3=4스테이지
void generate_reverse_item(int stage_num) {
    if (stage_num < 2) return;   // 1,2 스테이지에서는 생성하지 않음

    int rx, ry;
    do {
        rx = rand() % 30;
        ry = rand() % 30;
    } while (map[stage_num][ry][rx] != 0);

    map[stage_num][ry][rx] = 8;
}