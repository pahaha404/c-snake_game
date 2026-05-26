#include "snake.h"
#include "game.h"   // SnakeGame 정의, gate.h의 자유 함수도 함께 가져옴
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
    int snake_location[4][3][2] = {
        {{4, 23}, {6, 8}, {18, 16}},
        {{15, 14}, {21, 9}, {3, 18}},
        {{4, 15}, {27, 18}, {16, 15}},
        {{16, 15}, {6, 12}, {27, 17}}
    };

    if(game->stage_num > 0){
        for(int i=0; i<(int)snake.size()-1; i++){
            map[game->stage_num][snake[i].y][snake[i].x] = 0;
        }
        snake.clear();
        refresh();
    }

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

    if(move_posX < 0 || move_posX >= 30 || move_posY < 0 || move_posY >= 30){
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }

    if(map[stage_num][move_posY][move_posX] == 5){
        Growth_item += 1;
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
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
    // 노란 gate
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
    // 파란 gate
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
    else if(map[stage_num][move_posY][move_posX] == 1 ||
            map[stage_num][move_posY][move_posX] == 2 ||
            map[stage_num][move_posY][move_posX] == 4){
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }
    else{
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

