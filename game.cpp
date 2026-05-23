#include "window.cpp"
#include "gate.cpp"
#include "snake.cpp"
#include "item.cpp"


void game(){
  while(true){
    time_t start, end;
    Head_Direction = 1;
    set_zero();
    color();
    make_snake();
    nodelay(stdscr, true); //delay 없이 움직이게 하기
    while(true){
      // item, gate 초기화
      for(int i=0;i<30;i++){
        for(int j=0; j<30; j++){                                      //여기 바로 아래  '|| map[stage_num][i][j] == 8'추가함                              
          if(map[stage_num][i][j] == 5 || map[stage_num][i][j] == 6 || map[stage_num][i][j] == 8)  map[stage_num][i][j] = 0;
          if(map[stage_num][i][j] == 7 || map[stage_num][i][j] == 11) map[stage_num][i][j] = 1;
        }
      }
      gate.clear();
      blue_gate.clear();
      refresh();

      generate_item(); // item 생성
      generate_gate(); // gate 생성
      color();

      start = time(NULL);
while(true){
  Head_Direction = set_Head_Direction();
  Head_Direction = move_Snake();
  color();
  current_speed_level = stage_num + 1; // stage 0일 때 레벨 1, stage 3일 때 레벨 4
  score(); // 이제 score() 함수가 실행될 때 바뀐 속도 레벨을 인식할 수 있습니다.
  int base_delay = 400000;          // 기본 딜레이 (0.4초)로 증가시킴
  int speed_up = stage_num * 40000; // 스테이지당 0.04초씩 딜레이 단축
  int final_delay = base_delay - speed_up;

  // 너무 빨라져서 게임이 터지는 걸 방지 (최소 딜레이 0.08초 보장)
  if (final_delay < 80000) {
      final_delay = 80000;
   }
  
  usleep(final_delay); // 계산된 동적 딜레이 적용
  end = time(NULL);
  
  if(end - start >= 15){
    stage_num++;
    stage_flag = 1;
    break;
  }
}
      if(stage_flag == 1){
        break;
      }
    }
    if(stage_num == 4){
      NEXTGAME(2);
      break;
    }
    NEXTGAME(1);
  }
}
