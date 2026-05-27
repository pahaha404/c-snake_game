/**
 * @file item.cpp
 * @brief Snake Game의 아이템 스폰 관리 구현 파일
 * @details Food와 Poison 클래스를 인스턴스화하여 Growth/Poison 아이템을 랜덤 생성하고,
 *          Reverse Direction Item(8번) 스폰 로직을 조율합니다.
 * @author Hansol
 * @date 2026-05-28
 */
#include "game.h"
#include "food.h"
#include "poison.h"
#include <cstdlib>
#include <time.h>

void SnakeGame::generate_item(){
  srand((unsigned) time(0));

  // Food 및 Poison 객체를 사용하여 각각 아이템 스폰
  Food growth_food;
  growth_food.spawn(stage_num);

  Poison poison_item;
  poison_item.spawn(stage_num);

  // ── 추가: Reverse Direction Item 생성 ──
  int rx, ry;
  do {
      rx = rand() % 30;
      ry = rand() % 30;
  } while (map[stage_num][ry][rx] != 0);
  map[stage_num][ry][rx] = 8;
  //추가 끝
}
