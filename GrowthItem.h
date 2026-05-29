#ifndef GROWTHITEM_H
#define GROWTHITEM_H

// Growth Item 생성 (map 값 5) — 빈칸(0)에만 생성
//   map 은 전역 배열(map.h)이라 그대로 접근 가능,
//   클래스 멤버 중 필요한 건 stage_num 뿐이라 인자로 받는다.
void generate_growth_item(int stage_num);

#endif
