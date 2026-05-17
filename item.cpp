#include "game.h"

#include "game.h"

void generate_item(){
  srand((unsigned) time(0));

  int gx, gy;
  int px, py;

  // Growth Item 생성: 반드시 빈칸(0)에만 생성
  do {
    gx = rand() % 30;
    gy = rand() % 30;
  } while(map[stage_num][gy][gx] != 0);

  map[stage_num][gy][gx] = 5;

  // Poison Item 생성: 반드시 빈칸(0)에만 생성
  // Growth Item과 같은 위치도 방지
  do {
    px = rand() % 30;
    py = rand() % 30;
  } while(map[stage_num][py][px] != 0);

  map[stage_num][py][px] = 6;
}
