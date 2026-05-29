/**
 * @file game.cpp
 * @brief Snake Game의 메인 게임 진행 루프 구현 파일
 * @details 스테이지 초기화, 동적 속도 난이도 지연 처리 및
 *          스테이지 클리어/실패 플로우 제어를 수행합니다.
 * @author Hansol
 * @date 2026-05-28
 */
#include "game.h"

void SnakeGame::game(){
    while(true){
        time_t start, end;
        Head_Direction = 1;                  // alias 덕분에 그대로 동작
        set_zero();
        color();
        snake_obj.make_snake();              // ★

        nodelay(stdscr, true);
        while(true){
            for(int i=0;i<30;i++){
                for(int j=0; j<30; j++){
                    if(map[stage_num][i][j] == 5 || map[stage_num][i][j] == 6 || map[stage_num][i][j] == 8)
                        map[stage_num][i][j] = 0;
                    if(map[stage_num][i][j] == 7 || map[stage_num][i][j] == 11)
                        map[stage_num][i][j] = 1;
                }
            }
            gate.clear();
            blue_gate.clear();
            refresh();

            generate_item();
            generate_gate();
            color();

            start = time(NULL);
            while(true){
                // 한 프레임마다 입력 방향을 갱신한 뒤 실제 뱀 이동과 충돌 처리를 수행합니다.
                Head_Direction = snake_obj.set_Head_Direction();
                Head_Direction = snake_obj.move_Snake();

                // 이동 결과를 화면과 점수판에 반영하고, 현재 스테이지를 속도 단계로 표시합니다.
                color();
                current_speed_level = stage_num + 1;
                score();

                // 스테이지가 올라갈수록 프레임 지연 시간을 줄여 난이도를 높입니다.
                const int base_delay = 400000;
                const int speed_up   = stage_num * 40000;
                int final_delay = base_delay - speed_up;
                if (final_delay < 80000) final_delay = 80000;
                usleep(final_delay);
                end = time(NULL);

                // 제한 시간이 지나면 다음 스테이지로 넘어가도록 플래그를 세우고 루프를 종료합니다.
                if(end - start >= 15){
                    stage_num++;
                    stage_flag = 1;
                    break;
                }
            }
            if(stage_flag == 1) break;
        }
        if(stage_num == 4){
            NEXTGAME(2);
            break;
        }
        NEXTGAME(1);
    }
}

