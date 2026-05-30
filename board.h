/**
 * @file board.h
 * @brief 맵 데이터 객체화(Board) 클래스 정의 헤더
 * @details 30x30 크기의 4개 스테이지별 맵 정보 데이터를 캡슐화하며,
 *          이전 배열 참조 호환을 위한 전역 매크로 오버라이드를 제공합니다.
 * @author Hansol
 * @date 2026-05-28
 */
#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    int map[4][30][30];
    Board();
};

extern Board board_obj;

// 기존 코드와의 100% 호환성을 유지하기 위한 매크로 정의
#define map board_obj.map

#endif
