#include "game.h"

void SnakeGame::game() {
    while (true) {
        time_t start, end;
        Head_Direction = 1;                  // alias 덕분에 그대로 동작
        set_zero();
        color();
        snake_obj.make_snake();              // ★

        nodelay(stdscr, true);
        while (true) {
            // 매 사이클 시작: 직전 사이클의 아이템 자취(5/6/8/9)는 빈칸으로,
            // 게이트 자리(7/11)는 다시 일반 벽(1) 으로 복원한다. 이로써 다음
            // 아이템·게이트 재배치가 항상 깨끗한 상태에서 이루어진다.
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
            generate_growth_item(stage_num);
            generate_poison_item(stage_num);
            generate_reverse_item(stage_num);
            generate_golden_apple(stage_num);
            generate_gate();
            color();

            start = time(NULL);
            while (true) {
                Head_Direction = snake_obj.set_Head_Direction();   // ★
                Head_Direction = snake_obj.move_Snake();           // ★
                color();
                current_speed_level = stage_num + 1;
                score();
                int base_delay = 400000;
                int speed_up = stage_num * 40000;
                int final_delay = base_delay - speed_up;
                if (final_delay < 80000) final_delay = 80000;
                usleep(final_delay);
                end = time(NULL);

                if (end - start >= 15) {
                    stage_num++;
                    stage_flag = 1;
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