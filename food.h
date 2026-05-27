/**
 * @file food.h
 * @brief Growth Item(먹이) 관리를 위한 Food 클래스 정의 헤더
 * @author Hansol
 * @date 2026-05-28
 */
#ifndef FOOD_H
#define FOOD_H

class Food {
public:
    int x, y;
    Food();
    void spawn(const int stage_num);
};

#endif
