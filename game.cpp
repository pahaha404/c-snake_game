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
            //generate_item(); 변경
            srand((unsigned)time(0));
            generate_growth_item(stage_num);
            generate_poison_item(stage_num);
            generate_reverse_item(stage_num);
            //고침
            generate_gate();
            color();

            start = time(NULL);
            while(true){
                Head_Direction = snake_obj.set_Head_Direction();   // ★
                Head_Direction = snake_obj.move_Snake();           // ★
                color();
                current_speed_level = stage_num + 1;
                score();
                int base_delay = 400000;
                int speed_up   = stage_num * 40000;
                int final_delay = base_delay - speed_up;
                if (final_delay < 80000) final_delay = 80000;
                usleep(final_delay);
                end = time(NULL);

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

