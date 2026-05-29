/**
 * snake.cpp
 * Snake 객체의 초기화, 입력 처리, 이동 및 충돌 로직을 구현하는 파일
 * 뱀의 몸통 좌표 벡터를 갱신하면서 보드 map에 머리와 몸통 상태를 반영합니다.
 * Growth/Poison/Reverse 아이템 효과, 벽 및 자기 몸 충돌, 노란색/파란색 게이트 통과처럼
 * 한 턴 이동 중 발생할 수 있는 규칙을 처리합니다.
 */
#include "snake.h"
#include "game.h"
#include <cstdlib>

Snake::Snake() {
    Head_Direction = 1;
    Body_length    = 0;
    Growth_item    = 0;
    Poison_item    = 0;
    Reverse_active = 0;
    Reverse_start  = 0;
    key_to_dir[0] = 0;
    key_to_dir[1] = 1;
    key_to_dir[2] = 2;
    key_to_dir[3] = 3;
    game = nullptr;
}

void Snake::make_snake(){
    // 각 스테이지마다 벽과 겹치지 않는 시작 후보 좌표 3개를 준비합니다.
    static const int snake_location[4][3][2] = {
        {{4, 23}, {6, 8}, {18, 16}},
        {{15, 14}, {21, 9}, {3, 18}},
        {{4, 15}, {27, 18}, {16, 15}},
        {{16, 15}, {6, 12}, {27, 17}}
    };

    // 다음 스테이지로 넘어온 경우 이전 스테이지에 남아 있던 뱀 흔적을 지웁니다.
    if(game->stage_num > 0){
        for(int i=0; i<(int)snake.size()-1; i++){
            map[game->stage_num][snake[i].y][snake[i].x] = 0;
        }
        snake.clear();
        refresh();
    }

    // 후보 중 하나를 무작위로 선택한 뒤 길이 4의 초기 뱀을 가로 방향으로 배치합니다.
    srand((unsigned) time(0));
    int a = rand()%3;
    for(int i=0; i<4; i++)
        snake.push_back(snakepart(snake_location[game->stage_num][a][1]+i,
                                  snake_location[game->stage_num][a][0]));

    map[game->stage_num][snake[0].y][snake[0].x] = 3;
    for(int i=1; i<4; i++)
        map[game->stage_num][snake[i].y][snake[i].x] = 4;
    refresh();
    game->color();
    getch();
}

int Snake::set_Head_Direction() {
    
    if (Reverse_active && time(NULL) - Reverse_start >= 10) {
        Reverse_active = 0;
        key_to_dir[0] = 0; key_to_dir[1] = 1;
        key_to_dir[2] = 2; key_to_dir[3] = 3;
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

    // 방향 인덱스의 반대 방향 쌍은 합이 3이므로, 즉시 180도 회전하면 게임 오버입니다.
    if (new_dir + Head_Direction == 3) {
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }

    Head_Direction = new_dir;
    return Head_Direction;
}

int Snake::move_Snake(){
    int stage_num = game->stage_num;
    int move_posX = snake[0].x + head_way[Head_Direction][0];
    int move_posY = snake[0].y + head_way[Head_Direction][1];

    // 보드 범위를 벗어나는 이동은 벽 충돌과 동일하게 게임 오버로 처리합니다.
    if(move_posX < 0 || move_posX >= 30 || move_posY < 0 || move_posY >= 30){
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }

    // Growth 아이템: 새 머리만 추가하고 꼬리는 제거하지 않아 길이가 1 증가합니다.
    if(map[stage_num][move_posY][move_posX] == 5){
        Growth_item += 1;
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // Poison 아이템: 이동하면서 꼬리를 두 칸 제거하므로 전체 길이가 1 감소합니다.
    else if(map[stage_num][move_posY][move_posX] == 6){
        if(snake.size() == 3){
            game->NEXTGAME(3);
            game->del_win();
            exit(0);
        }
        Poison_item += 1;
        map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
        map[stage_num][snake[snake.size()-2].y][snake[snake.size()-2].x] = 0;
        snake.pop_back();
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // Reverse 아이템: 일정 시간 동안 방향키 매핑을 섞어 조작을 어렵게 만듭니다.
    else if (map[stage_num][move_posY][move_posX] == 8) {
        Reverse_active = 1;
        Reverse_start  = time(NULL);

        int dirs[3] = { 0, 1, 2 };
        for (int i = 2; i > 0; i--) {
            int j = rand() % (i + 1);
            int t = dirs[i]; dirs[i] = dirs[j]; dirs[j] = t;
        }
        key_to_dir[0] = dirs[0];
        key_to_dir[1] = dirs[1];
        key_to_dir[2] = dirs[2];
        key_to_dir[3] = 3;

        map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // 노란색 게이트: 반대편 게이트의 출구 방향을 계산하고 일반 이동처럼 꼬리를 줄입니다.
    else if(map[stage_num][move_posY][move_posX] == 7 && game->collision_gate() == 7){
        Head_Direction = game->pass_the_gate();
        game->gate_posX = get_yellow_exit_x() + head_way[Head_Direction][0];   // 자유 함수
        game->gate_posY = get_yellow_exit_y() + head_way[Head_Direction][1];

        map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(game->gate_posX, game->gate_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // 파란색 게이트: 출구로 이동하면서 Growth_item을 증가시키고 길이를 유지하지 않고 늘립니다.
    else if(map[stage_num][move_posY][move_posX] == 11 && game->collision_gate() == 11){
        Head_Direction = game->pass_the_blue_gate();
        game->gate_posX = get_blue_exit_x() + head_way[Head_Direction][0];     // 자유 함수
        game->gate_posY = get_blue_exit_y() + head_way[Head_Direction][1];

        snake.insert(snake.begin(), snakepart(game->gate_posX, game->gate_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;

        Growth_item += 1;
        refresh();
    }
    // 벽, Immune Wall, 몸통과 충돌하면 게임 오버입니다.
    else if(map[stage_num][move_posY][move_posX] == 1 ||
            map[stage_num][move_posY][move_posX] == 2 ||
            map[stage_num][move_posY][move_posX] == 4){
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }
    else{
        // 게이트에서 빠져나온 꼬리까지 모두 통과하면 활성 게이트를 종료합니다.
        if(game->gate_posX != 0 &&
           snake[snake.size()-1].x == game->gate_posX &&
           snake[snake.size()-1].y == game->gate_posY){
            game->finish_active_gate();
            game->gate_posX = 0;
            game->gate_posY = 0;
            refresh();
        }
        map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }

    return Head_Direction;
}
