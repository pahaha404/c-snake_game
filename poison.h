/**
 * @file poison.h
 * @brief Poison Item(독) 관리를 위한 Poison 클래스 정의 헤더
 * @author Hansol
 * @date 2026-05-28
 */
#ifndef POISON_H
#define POISON_H

class Poison {
public:
    int x, y;
    Poison();
    void spawn(const int stage_num);
};

#endif
