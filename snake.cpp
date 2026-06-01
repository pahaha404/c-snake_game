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
    Body_length = 0;
    Growth_item = 0;
    Poison_item = 0;
    Reverse_active = 0;
    Reverse_start = 0;
    pending_growth = 0;
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
    // 이전 스테이지에서 남아 있을 수 있는 Golden Apple 잔여 성장은
    // 스테이지를 넘기는 순간 끊는다.
    pending_growth = 0;

    // 후보 중 하나를 무작위로 선택한 뒤 길이 3의 초기 뱀을 가로 방향으로 배치합니다.
    srand((unsigned) time(0));
    int a = rand()%3;
    for(int i=0; i<3; i++)
        snake.push_back(snakepart(snake_location[game->stage_num][a][1]+i,
                                  snake_location[game->stage_num][a][0]));
    map[game->stage_num][snake[0].y][snake[0].x] = 3;
    game->visited[snake[0].y][snake[0].x] = true;
    for (int i = 1; i < 3; i++)
        map[game->stage_num][snake[i].y][snake[i].x] = 4;
    refresh();
    game->color();
    getch();
}

// -----------------------------------------------------------------------------
// Reverse Item 효과의 핵심 동작.
//   "진행방향에 수직"인 두 화살표 키의 기능을 서로 맞바꾼다. 진행방향과
//   평행한 키(전진/후진) 는 그대로 두기 때문에, 반대 방향 입력 시
//   set_Head_Direction() 의 (new_dir + Head_Direction == 3) 게임오버 판정이
//   변하지 않는다는 점이 중요하다.
//
//     - 위/아래로 진행(0,3) → ←/→ 화살표를 서로 교환
//     - 좌/우로 진행(1,2)   → ↑/↓ 화살표를 서로 교환
//
//   진행방향이 바뀔 때마다 기준도 다시 잡혀야 하므로, set_Head_Direction()
//   에서 Reverse_active 동안 매 틱 호출한다.
//   방향 상수: UP=0, LEFT=1, RIGHT=2, DOWN=3
// -----------------------------------------------------------------------------
void Snake::update_reverse_keys() {
    if (Head_Direction == 0 || Head_Direction == 3) {
        key_to_dir[0] = 0;
        key_to_dir[3] = 3;
        key_to_dir[1] = 2;
        key_to_dir[2] = 1;
    }
    else {
        key_to_dir[1] = 1;
        key_to_dir[2] = 2;
        key_to_dir[0] = 3;
        key_to_dir[3] = 0;
    }
}

int Snake::set_Head_Direction() {
    // Reverse Item 활성 동안의 처리:
    //   - 10초 경과 시 키 매핑을 원상복구하고 효과를 종료한다.
    //   - 그렇지 않으면 현재 진행방향에 맞춰 매 틱 매핑을 갱신한다.
    if (Reverse_active) {
        if (time(NULL) - Reverse_start >= 10) {
            Reverse_active = 0;
            key_to_dir[0] = 0; key_to_dir[1] = 1;
            key_to_dir[2] = 2; key_to_dir[3] = 3;
        }
        else {
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

    // 방향 인덱스의 반대 방향 쌍은 합이 3이므로, 즉시 180도 회전하면 게임 오버입니다.
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

    // 보드 범위를 벗어나는 이동은 벽 충돌과 동일하게 게임 오버로 처리합니다.
    if(move_posX < 0 || move_posX >= 30 || move_posY < 0 || move_posY >= 30){
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }

    // 이스터에그: 이미 가본 타일인지 체크하고 방문 표기
    if (game->visited[move_posY][move_posX]) {
        game->easter_egg_eligible = false;
    }
    game->visited[move_posY][move_posX] = true;

    // Growth Item(5): 머리만 추가하고 꼬리는 자르지 않아 길이 +1.
    if (map[stage_num][move_posY][move_posX] == 5) {
        Growth_item += 1;
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // Poison Item(6): 꼬리 두 칸 제거 + 새 머리 추가로 최종 길이 -1.
    // 단, 적용 전 길이가 3 인 경우 결과 길이가 너무 작아지므로 즉시 게임오버.
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
    // Reverse Item(8): 길이는 변하지 않고 통과한다. 효과 자체는 Reverse_active
    // 플래그를 켜고 update_reverse_keys() 로 키 매핑을 진행방향에 맞춰 즉시
    // 적용하는 것으로 시작된다. 지속시간 처리는 set_Head_Direction() 에서.
    else if (map[stage_num][move_posY][move_posX] == 8) {
        Reverse_active = 1;
        Reverse_start = time(NULL);
        update_reverse_keys();

        map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // Golden Apple(9): 길이 +3.
    //   한 틱에 머리만 추가해 즉시 +1 을 적용하고, pending_growth 에 2 를 더해
    //   다음 두 번의 일반 이동에서 꼬리를 자르지 않게 한다. 이렇게 분산해야
    //   세 칸이 같은 좌표에 겹치지 않고 진행하면서 자연스럽게 누적된다.
    else if (map[stage_num][move_posY][move_posX] == 9) {
        pending_growth += 2;
        Growth_item += 3; // 황금사과를 먹었을 때 성장 점수(+) 카운트도 대폭 +3 일시 가산!

        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }
    // 노란 gate
    else if (map[stage_num][move_posY][move_posX] == 7 && game->collision_gate() == 7) {
        Head_Direction = game->pass_the_gate();
        game->gate_posX = game->get_yellow_exit_x() + head_way[Head_Direction][0];
        game->gate_posY = game->get_yellow_exit_y() + head_way[Head_Direction][1];  //진출 위치에 머리 삽입, 꼬리 처리

        // 이스터에그: 게이트 탈출 출구 좌표 방문 검사 및 마크
        if (game->gate_posX >= 0 && game->gate_posX < 30 && game->gate_posY >= 0 && game->gate_posY < 30) {
            if (game->visited[game->gate_posY][game->gate_posX]) {
                game->easter_egg_eligible = false;
            }
            game->visited[game->gate_posY][game->gate_posX] = true;
        }

        map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
        snake.pop_back();
        snake.insert(snake.begin(), snakepart(game->gate_posX, game->gate_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        
        game->Gate_cnt += 1; // 실시간 가산: 노란 게이트 진입 즉시 G 카운트 상승!
        refresh();
    }
    // 파란 gate
    else if (map[stage_num][move_posY][move_posX] == 11 && game->collision_gate() == 11) {
        Head_Direction = game->pass_the_blue_gate();
        game->gate_posX = game->get_blue_exit_x() + head_way[Head_Direction][0];
        game->gate_posY = game->get_blue_exit_y() + head_way[Head_Direction][1];

        // 이스터에그: 게이트 탈출 출구 좌표 방문 검사 및 마크
        if (game->gate_posX >= 0 && game->gate_posX < 30 && game->gate_posY >= 0 && game->gate_posY < 30) {
            if (game->visited[game->gate_posY][game->gate_posX]) {
                game->easter_egg_eligible = false;
            }
            game->visited[game->gate_posY][game->gate_posX] = true;
        }

        snake.insert(snake.begin(), snakepart(game->gate_posX, game->gate_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;

        Growth_item += 1;
        game->Gate_cnt += 1; // 실시간 가산: 파란 게이트 진입 즉시 G 카운트 상승!
        refresh();
    }
    else if (map[stage_num][move_posY][move_posX] == 1 ||
        map[stage_num][move_posY][move_posX] == 2 ||
        map[stage_num][move_posY][move_posX] == 4) {
        game->NEXTGAME(3);
        game->del_win();
        exit(0);
    }
    // 일반 이동: 길이를 유지하며 한 칸 전진. 단, Golden Apple 잔여 성장이
    // 남아 있는 동안엔 이번 틱의 꼬리 자르기를 건너뛰어 길이를 늘린다.
    else {
        if (game->gate_posX != 0 &&
            snake[snake.size() - 1].x == game->gate_posX &&
            snake[snake.size() - 1].y == game->gate_posY) {
            game->finish_active_gate();
            game->gate_posX = 0;
            game->gate_posY = 0;
            refresh();
        }
        if (pending_growth > 0) {
            pending_growth--;
        }
        else {
            map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
            snake.pop_back();
        }
        snake.insert(snake.begin(), snakepart(move_posX, move_posY));
        map[stage_num][snake[0].y][snake[0].x] = 3;
        map[stage_num][snake[1].y][snake[1].x] = 4;
        refresh();
    }

    return Head_Direction;
}
