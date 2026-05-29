#include "game.h"

// Golden Apple 전용 커스텀 색 슬롯. 8(GRAY), 9(BLACK_A) 는 game.h 에서 사용 중.
#define COLOR_GOLD 10

void SnakeGame::color_init() {
    // 셀 값과 색 페어 인덱스의 대응:
    //   1=Wall, 2=Immune Wall, 3=Snake Head, 4=Snake Body, 5=Growth(녹),
    //   6=Poison(적), 7=Yellow Gate, 8=Blank, 9=Reverse(마젠타), 11=Blue Gate,
    //   10=Golden Apple (init_color 로 정의한 커스텀 골드 톤)
    init_color(COLOR_GRAY, 150, 150, 150);
    init_pair(1, COLOR_GRAY, COLOR_GRAY);
    init_color(COLOR_BLACK_A, 0, 0, 0);
    init_pair(2, COLOR_BLACK_A, COLOR_BLACK_A);
    init_pair(3, COLOR_CYAN, COLOR_CYAN);
    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_GREEN, COLOR_GREEN);     // Growth Item
    init_pair(6, COLOR_RED, COLOR_RED);         // Poison Item
    init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);
    init_pair(9, COLOR_MAGENTA, COLOR_MAGENTA); // Reverse Direction Item
    init_pair(11, COLOR_BLUE, COLOR_BLUE);

    // Golden Apple: 표준 8색에는 진짜 골드가 없어, ncurses 의 init_color 로
    // RGB(1000, 843, 0) 톤을 새 슬롯에 직접 정의해 사용한다.
    init_color(COLOR_GOLD, 1000, 843, 0);
    init_pair(10, COLOR_GOLD, COLOR_GOLD);      // Golden Apple
}

void SnakeGame::color() {
    // 셀 값(0~9) → 색 페어 인덱스 매핑 테이블.
    // 셀 값 11(Blue Gate) 은 범위 밖이라 아래에서 별도로 처리한다.
    int pair_map[] = { 8, 1, 2, 3, 4, 5, 6, 7, 9, 10 };

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            int val = map[stage_num][i][j];
            if (val >= 0 && val <= 9) {
                int pair = pair_map[val];

                // Reverse Item 지속 동안 몸통(셀 값 4) 을 아이템과 동일한
                // 마젠타(pair 9) 로 그려, 효과가 켜져 있음을 시각적으로 알린다.
                // 머리(3) 와 다른 칸은 영향을 받지 않는다.
                if (Reverse_active && val == 4) pair = 9;

                wattron(win1, COLOR_PAIR(pair));
                mvwprintw(win1, i, j, " ");
                wattroff(win1, COLOR_PAIR(pair));
            }
            else if (val == 11) {
                wattron(win1, COLOR_PAIR(11));
                mvwprintw(win1, i, j, " ");
                wattroff(win1, COLOR_PAIR(11));
            }
        }
    }
    wrefresh(win1);
}

void SnakeGame::score() {
    // Body_length, Growth_item, Poison_item, Gate_cnt
    int goal[4][4] = { {5, 2, 1, 1}, {10, 4, 2, 2}, {15, 6, 4, 3}, {15, 6, 5, 4} };
    // 목표 점수 표시
    wborder(win2, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(win2, 1, 6, "*Mission*");
    mvwprintw(win2, 3, 8, "B : %d", goal[stage_num][0]);
    mvwprintw(win2, 5, 8, "+ : %d", goal[stage_num][1]);
    mvwprintw(win2, 7, 8, "- : %d", goal[stage_num][2]);
    mvwprintw(win2, 9, 8, "G : %d", goal[stage_num][3]);
    mvwprintw(win3, 11, 2, "SPEED :    %d", current_speed_level);

    int size = snake.size();
    int current_size = snake.size();
    Body_length = max(size, Body_length); // max_size

    // 점수 표시
    wborder(win3, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(win3, 1, 7, "*Score*");
    mvwprintw(win3, 3, 2, "B : %d / %d", current_size, Body_length);
    mvwprintw(win3, 5, 2, "+ :   %d   ", Growth_item);
    mvwprintw(win3, 7, 2, "- :   %d   ", Poison_item);
    mvwprintw(win3, 9, 2, "G :   %d   ", Gate_cnt);

    if (Body_length < goal[stage_num][0])  mvwprintw(win3, 3, 14, "(   )");
    else  mvwprintw(win3, 3, 14, "( V )");

    if (Growth_item < goal[stage_num][1])  mvwprintw(win3, 5, 14, "(   )");
    else  mvwprintw(win3, 5, 14, "( V )");

    if (Poison_item < goal[stage_num][2])  mvwprintw(win3, 7, 14, "(   )");
    else  mvwprintw(win3, 7, 14, "( V )");

    if (Gate_cnt < goal[stage_num][3]) mvwprintw(win3, 9, 14, "(   )");
    else  mvwprintw(win3, 9, 14, "( V )");

    if ((Body_length >= goal[stage_num][0]) && (Growth_item >= goal[stage_num][1]) &&
        (Poison_item >= goal[stage_num][2]) && (Gate_cnt >= goal[stage_num][3])) {
        stage_num++;
        stage_flag = 1;
    }

    wrefresh(win2);
    wrefresh(win3);
}

void SnakeGame::set_zero() {
    stage_flag = 0;
    Body_length = 0;
    Growth_item = 0;
    Poison_item = 0;
    Gate_cnt = 0;
    gate_posX = 0;
    gate_posY = 0;
    // 새 스테이지 시작 시 Reverse Item 상태와 키 매핑을 기본값으로 되돌린다.
    Reverse_active = 0;
    key_to_dir[0] = 0; key_to_dir[1] = 1;
    key_to_dir[2] = 2; key_to_dir[3] = 3;
}

void SnakeGame::NEXTGAME(int num) {
    wclear(win1);
    if (num == 1) {
        mvwprintw(win1, 10, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
        mvwprintw(win1, 11, 24, "★      STAGE CLEAR      ★");
        mvwprintw(win1, 12, 24, "★        ٩( ᐛ )و        ★");
        mvwprintw(win1, 13, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
    }
    else if (num == 2) {
        mvwprintw(win1, 10, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
        mvwprintw(win1, 11, 24, "★     STAGE ALL CLEAR   ★");
        mvwprintw(win1, 12, 24, "★       ( ღ 'ᴗ'ღ )      ★");
        mvwprintw(win1, 13, 24, "★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★ ★");
    }
    else if (num == 3) {
        mvwprintw(win1, 10, 28, "+ - - - - - - - - - - - +");
        mvwprintw(win1, 11, 28, "|    G A M E O V E R    |");
        mvwprintw(win1, 12, 28, "|        ( ಥ﹏ಥ)        |");
        mvwprintw(win1, 13, 28, "+ - - - - - - - - - - - +");
    }
    wrefresh(win1);
    nodelay(stdscr, false);
    getch();
    wclear(win1);
}

void SnakeGame::del_win() {
    delwin(win3);
    delwin(win2);
    delwin(win1);
    endwin();
}