#include "game.h"

// 스네이크 생성
void make_snake(){
  int snake_location[4][3][2] = {
    {{4, 23}, {6, 8}, {18, 16}},
    {{15, 14}, {21, 9}, {3, 18}},
    {{4, 15}, {27, 18}, {16, 15}},
    {{16, 15}, {6, 12}, {27, 17}}
  };

  if(stage_num > 0){
    for(int i=0; i<(int)snake.size()-1; i++){
      map[stage_num][snake[i].y][snake[i].x] = 0;
    }
    snake.clear();
    refresh();
  }

  srand((unsigned) time(0)); // 항상 다른 난수
  int a = rand()%3;
  for(int i=0; i<4; i++)
    snake.push_back(snakepart(snake_location[stage_num][a][1]+i, snake_location[stage_num][a][0]));

  map[stage_num][snake[0].y][snake[0].x] = 3;
  for(int i=1; i<4; i++)
    map[stage_num][snake[i].y][snake[i].x] = 4;
  refresh();
  color();
  getch();
}

//방향키 설정        ---------------전체 수정------
int set_Head_Direction() {
    // 10초 경과 시 매핑 원복
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
    default: return Head_Direction;  // 키 입력 없음 → 그대로
    }

    // 반대방향 검사: 반대쌍은 합이 3 (0↔3, 1↔2)
    if (new_dir + Head_Direction == 3) {
        NEXTGAME(3);
        del_win();
        exit(0);
    }

    Head_Direction = new_dir;
    return Head_Direction;
}//----------------수정 끝

int move_Snake(){
  // 스네이크 다음 이동 위치
  int move_posX = snake[0].x + head_way[Head_Direction][0];
  int move_posY = snake[0].y + head_way[Head_Direction][1];

  if(move_posX < 0 || move_posX >= 30 || move_posY < 0 || move_posY >= 30){
    NEXTGAME(3);
    del_win();
    exit(0);
  }

  // growth item 먹을 때
  if(map[stage_num][move_posY][move_posX] == 5){
    // 머리 + 1
    Growth_item += 1;
    snake.insert(snake.begin(), snakepart(move_posX, move_posY));
    map[stage_num][snake[0].y][snake[0].x] = 3;
    map[stage_num][snake[1].y][snake[1].x] = 4;
    refresh();
  }

  // poison item 먹을 때
  else if(map[stage_num][move_posY][move_posX] == 6){
    // 머리 + 1, 꼬리 - 2
    if(snake.size() == 3){
      NEXTGAME(3);
      del_win();
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

  // ── 추가: reverse direction item 먹을 때 ──
  else if (map[stage_num][move_posY][move_posX] == 8) {
      Reverse_active = 1;
      Reverse_start = time(NULL);

      // KEY_UP/LEFT/RIGHT 3개 키의 매핑을 셔플 (Fisher-Yates)
      int dirs[3] = { 0, 1, 2 };
      for (int i = 2; i > 0; i--) {
          int j = rand() % (i + 1);
          int t = dirs[i]; dirs[i] = dirs[j]; dirs[j] = t;
      }
      key_to_dir[0] = dirs[0];  // KEY_UP
      key_to_dir[1] = dirs[1];  // KEY_LEFT
      key_to_dir[2] = dirs[2];  // KEY_RIGHT
      key_to_dir[3] = 3;        // KEY_DOWN은 그대로

      // 일반 이동 (머리 +1, 꼬리 -1)
      map[stage_num][snake[snake.size() - 1].y][snake[snake.size() - 1].x] = 0;
      snake.pop_back();
      snake.insert(snake.begin(), snakepart(move_posX, move_posY));
      map[stage_num][snake[0].y][snake[0].x] = 3;
      map[stage_num][snake[1].y][snake[1].x] = 4;
      refresh();
  }//  추가 끝

  // 노란 gate를 만났을 때
  else if(map[stage_num][move_posY][move_posX] == 7 && collision_gate() == 7){
    Head_Direction = pass_the_gate();

    gate_posX = get_yellow_exit_x() + head_way[Head_Direction][0];
    gate_posY = get_yellow_exit_y() + head_way[Head_Direction][1];

    map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
    snake.pop_back();
    snake.insert(snake.begin(), snakepart(gate_posX, gate_posY));
    map[stage_num][snake[0].y][snake[0].x] = 3;
    map[stage_num][snake[1].y][snake[1].x] = 4;
    refresh();
  }

  // 파란 gate를 만났을 때
  else if(map[stage_num][move_posY][move_posX] == 11 && collision_gate() == 11){
    Head_Direction = pass_the_blue_gate();

    gate_posX = get_blue_exit_x() + head_way[Head_Direction][0];
    gate_posY = get_blue_exit_y() + head_way[Head_Direction][1];

    // body +1 효과: 꼬리를 pop하지 않음
    snake.insert(snake.begin(), snakepart(gate_posX, gate_posY));
    map[stage_num][snake[0].y][snake[0].x] = 3;
    map[stage_num][snake[1].y][snake[1].x] = 4;

    Growth_item += 1;
    refresh();
  }

  //벽을 만났을 때
  else if(map[stage_num][move_posY][move_posX] == 1 ||
          map[stage_num][move_posY][move_posX] == 2 ||
          map[stage_num][move_posY][move_posX] == 4){
    NEXTGAME(3);
    del_win();
    exit(0);
  }

  // 아무것도 만나지 않을 때
  else{
    // 스네이크 전체가 gate를 다 통과했을 때
    if(gate_posX != 0 && snake[snake.size()-1].x == gate_posX && snake[snake.size()-1].y == gate_posY){
      finish_active_gate();
      gate_posX = 0; gate_posY = 0;
      refresh();
    }

    // 머리 + 1, 꼬리 - 1
    map[stage_num][snake[snake.size()-1].y][snake[snake.size()-1].x] = 0;
    snake.pop_back();
    snake.insert(snake.begin(), snakepart(move_posX, move_posY));
    map[stage_num][snake[0].y][snake[0].x] = 3;
    map[stage_num][snake[1].y][snake[1].x] = 4;
    refresh();
  }

  return Head_Direction;
}
