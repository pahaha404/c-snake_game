// =============================================================================
// game.cpp
// -----------------------------------------------------------------------------
// Snake Game의 메인 게임 진행 루프 구현 파일.
// 스테이지 초기화, 동적 속도 난이도 지연 처리 및
// 스테이지 클리어/실패 플로우 제어를 수행합니다.
// =============================================================================
#include "game.h"

void SnakeGame::game() {
    while (true) {
        time_t start, end;
        Head_Direction = 1;                 
        set_zero();
        stage_start_time = time(NULL);
        color();
        snake_obj.make_snake();         

        nodelay(stdscr, true);
        while (true) {
            // 매 사이클 시작: 직전 사이클의 아이템 자취(5/6/8/9)는 빈칸으로,
            // 게이트 자리(7/11)는 다시 일반 벽(1) 으로 복원한다. 이로써 다음
            // 아이템,게이트 재배치가 항상 깨끗한 상태에서 이루어진다.
            for (int i = 0; i < 30; i++) {
                for (int j = 0; j < 30; j++) {
                    if (map[stage_num][i][j] == 5 || map[stage_num][i][j] == 6 ||
                        map[stage_num][i][j] == 8 || map[stage_num][i][j] == 9)
                        map[stage_num][i][j] = 0;
                    if (map[stage_num][i][j] == 7 || map[stage_num][i][j] == 11)
                        map[stage_num][i][j] = 1;
                }
            }
            gate.clear();
            blue_gate.clear();
            refresh();

            // 아이템 4종을 한 번의 시드 아래 순차 배치한다.
            // 각 generate_* 는 빈칸을 찾는 거부 표본추출을 쓰므로 앞서 놓인
            // 아이템과 자연히 겹치지 않는다. Golden Apple 만 내부에서 자체
            // 확률 게이트로 등장 빈도를 낮추고, Reverse Item 은 stage_num 가드로
            // 3 스테이지부터만 실제 배치된다.
            srand((unsigned)time(0));
            GrowthItem growth_item;
            growth_item.generate(stage_num);

            PoisonItem poison_item;
            poison_item.generate(stage_num);

            ReverseItem reverse_item;
            reverse_item.generate(stage_num);

            GoldenApple golden_apple;
            golden_apple.generate(stage_num);

            generate_gate();
            color();

            start = time(NULL);
            while(true){
                // 한 프레임마다 입력 방향을 갱신한 뒤 실제 뱀 이동과 충돌 처리를 수행
                Head_Direction = snake_obj.set_Head_Direction();
                Head_Direction = snake_obj.move_Snake();

                // 이동 결과를 화면과 점수판에 반영하고, 현재 스테이지를 속도 단계로 표시
                color();
                current_speed_level = stage_num + 1;
                score();

                // 스테이지가 올라갈수록 프레임 지연 시간을 줄여 난이도 높이기
                const int base_delay = 400000;
                const int speed_up   = stage_num * 40000;
                int final_delay = base_delay - speed_up;
                if (final_delay < 80000) final_delay = 80000;
                usleep(final_delay);
                end = time(NULL);

                if (stage_flag == 1) break;

                if (end - start >= 15) {
                    break;
                }
            }
            if (stage_flag == 1) break;
        }
        if (stage_num == 4) {
            NEXTGAME(2);
            break;
        }
        NEXTGAME(1);
    }
}