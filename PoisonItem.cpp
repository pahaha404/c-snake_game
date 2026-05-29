#include "PoisonItem.h"
#include "map.h"
#include <cstdlib>

// Poison Item 생성: 반드시 빈칸(0)에만 생성
//   (Growth Item과 같은 위치는 빈칸 검사(!= 0)로 자연히 방지됨)
void generate_poison_item(int stage_num) {
    int px, py;
    do {
        px = rand() % 30;
        py = rand() % 30;
    } while (map[stage_num][py][px] != 0);

    map[stage_num][py][px] = 6;
}