#include "snake.h"
#include "game.h"   
#include <cstdlib>

Snake::Snake() {
    Head_Direction = 1;
    Body_length = 0;
    Growth_item = 0;
    Poison_item = 0;
    Reverse_active = 0;
    Reverse_start = 0;
    key_to_dir[0] = 0;
    key_to_dir[1] = 1;
    key_to_dir[2] = 2;
    key_to_dir[3] = 3;
    game = nullptr;
}

void Snake::make_snake() {
    int snake_location[4][3][2] = {
        {{4, 23}, {6, 8}, {18, 16}},
        {{15, 14}, {21, 9}, {3, 18}},
        {{4, 15}, {27, 18}, {16, 15}},
        {{16, 15}, {6, 12}, {27, 17}}
    };

    if (game->stage_num > 0) {
        for (int i = 0; i < (int)snake.size() - 1; i++) {
            map[game->stage_num][snake[i].y][snake[i].x] = 0;
        }
        snake.clear();
        refresh();
    }

    srand((unsigned)time(0));
    int a = rand() % 3;
    for (int i = 0; i < 4; i++)
        snake.push_back(snakepart(snake_location[game->stage_num][a][1] + i,
            snake_location[game->stage_num][a][0]));

    map[game->stage_num][snake[0].y][snake[0].x] = 3;
    for (int i = 1; i < 4; i++)
        map[game->stage_num][snake[i].y][snake[i].x] = 4;
    refresh();
    game->color();
    getch();
}

// =============================================================================
// Reverse 아이템 동작:
//   진행방향에 "수직"인 두 화살표 키를 서로 바꾼다.
//   진행방향과 "평행"한 키(전진/후진)는 그대로 두어, 반대 방향 입력 시
//   기존의 게임오버(new_dir + Head_Direction == 3) 판정이 유지된다.
//   - 위/아래로 진행(0,3): ←/→ 교환
//   - 왼/오른쪽 진행(1,2): ↑/↓ 교환
//   set_Head_Direction() 에서 매 틱 호출하여 진행방향이 바뀔 때마다 갱신.
//   방향 상수: UP=0, LEFT=1, RIGHT=2, DOWN=3
// =============================================================================
void Snake::update_reverse_keys() {
    if (Head_Direction == 0 || Head_Direction == 3) {
        // 위/아래로 진행 중 -> 왼쪽/오른쪽 화살표 교환
        key_to_dir[0] = 0;  // ↑ -> 위
        key_to_dir[3] = 3;  // ↓ -> 아래
        key_to_dir[1] = 2;  // ← -> 오른쪽
        key_to_dir[2] = 1;  // → -> 왼쪽
    }
    else {
        // 왼쪽/오른쪽으로 진행 중 -> 위/아래 화살표 교환
        key_to_dir[1] = 1;  // ← -> 왼쪽
        key_to_dir[2] = 2;  // → -> 오른쪽
        key_to_dir[0] = 3;  // ↑ -> 아래
        key_to_dir[3] = 0;  // ↓ -> 위
    }
}

int Snake::set_Head_Direction() {
    if (Reverse_active) {
        if (time(NULL) - Reverse_start >= 10) {
            // 지속시간(10초) 종료 -> 키 매핑 원상복구
            Reverse_active = 0;
            key_to_dir[0] = 0; key_to_dir[1] = 1;
            key_to_dir[2] = 2; key_to_dir[3] = 3;
        }
        else {
            // 지속 중 -> 현재 진행방향 기준으로 좌우(수직) 키 교환을 매 틱 갱신
            update_reverse_keys();
        }
    }

    int key = getch();
    int new_dir;

    switch (key) {
    case KEY_UP:    new_dir = key_to_dir[0]; break;
    case KEY_LEFT:  new_dir = key_to_dir[1]; break;
    case KEY_RIGHT: new_dir = key_to_dir[2]; break;
    case KEY_DOWN:  new_dir = key_to_dir[3]; break;
    default: return Head_Direction;
    }

    if (new_dir + Head_Direction == 3) {
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }

    Head_Direction = new_dir;
    return Head_Direction;
}

int Snake::move_Snake() {
    int stage_num = game->stage_num;
    int move_posX = snake[0].x + head_way[Head_Direction][0];
    int move_posY = snake[0].y + head_way[Head_Direction][1];

    if (move_posX < 0 || move_posX >= 30 || move_posY < 0 || move_posY >= 30) {
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }

    if (map[stage_num][move_posY][move_posX] == 5) {
        Growth_item += 1;
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    else if (map[stage_num][move_posY][move_posX] == 6) {
        if (snake.size() == 3) {
            game->NEXTGAME(3);
            game->del_win();
            exit(0);
        }
        Poison_item += 1;
        map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
        map[stage_num][snake[snake.size() - 2].y][snake[snake.size() - 2].x] = 0;
        snake.pop_back();
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    else if (map[stage_num][move_posY][move_posX] == 8) {
        // Reverse Direction 아이템 획득
        Reverse_active = 1;
        Reverse_start = time(NULL);

        // 현재 진행방향 기준 좌우(수직) 키 교환 매핑 즉시 적용
        update_reverse_keys();

        map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }

    // 노란 gate
    else if (map[stage_num][move_posY][move_posX] == 7 && game->collision_gate() == 7) {
        Head_Direction = game->pass_the_gate();
        game->gate_posX = game->get_yellow_exit_x() + head_way[Head_Direction][0];
        game->gate_posY = game->get_yellow_exit_y() + head_way[Head_Direction][1];
 
        map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(game->gate_posX, game->gate_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // 파란 gate
    else if (map[stage_num][move_posY][move_posX] == 11 && game->collision_gate() == 11) {
        Head_Direction = game->pass_the_blue_gate();
        game->gate_posX = game->get_blue_exit_x() + head_way[Head_Direction][0];
        game->gate_posY = game->get_blue_exit_y() + head_way[Head_Direction][1];
 
        snake.insert(snake.begin(), snakepart(game->gate_posX, game->gate_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
 
        Growth_item += 1;
        refresh();
    }
    else if (map[stage_num][move_posY][move_posX] == 1 ||
        map[stage_num][move_posY][move_posX] == 2 ||
        map[stage_num][move_posY][move_posX] == 4) {
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }
    else {
        if (game->gate_posX != 0 &&
            snake[snake.size() - 1].x == game->gate_posX &&
            snake[snake.size() - 1].y == game->gate_posY) {
            game->finish_active_gate();
            game->gate_posX = 0;
            game->gate_posY = 0;
            refresh();
        }
        map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }

    return Head_Direction;
}