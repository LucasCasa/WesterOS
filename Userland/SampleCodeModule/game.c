#include "lib.h"
#define  RADIUS 5
#define PLAYERS 3
extern double _sin(int angle);
extern double _cos(int angle);
char board[1024*768];
void get_key_up(uint8_t);
void get_key_down(uint8_t);
int mod[PLAYERS] = {0,0,0};
uint32_t last[] = {1,2,3};
uint32_t next[] = {5,6,7};
int ttinv[PLAYERS] = {255,255,255};
int tsinv[PLAYERS] = {0,0,0};
int erasable[PLAYERS] = {-1,-1,-1};

char draw_into_board(uint32_t pn,Point p);
void exit_game();

void game(){
  //CANTIDAD DE JUGADORES
  srand(*((uint32_t*)0xB8078));
  Point p[] = {{20,20},{1000,700},{1000,400}};
  Color c[] = {{255,0,0},{0,0,255},{0,255,0}};
  Color c1h = {100,0,0};
  for(int i = 0; i<1024*768;i++){
    board[i] = 0;
  }

  Color c2h = {0,0,100};
  double accumx[PLAYERS];
  double accumy[PLAYERS];
  for(int i = 0; i<PLAYERS;i++){
    accumx[i] = p[i].x;
    accumy[i] = p[i].y;
    erasable[i] = -1;
    mod[i] = 0;
    ttinv[i] = 255;
    tsinv[i] = 0;
  }

  _call_int80(INT_ENTER_DRAW_MODE);
  _call_int80(INT_CLEAR);
  //_call_int80(INT_EXIT_DRAW_MODE);

  char game_over[PLAYERS] = {0,0,0};

  int pass = 0;
  int ch = 0;

  int angle[PLAYERS] = {0,180,-90};

  _call_int80(INT_SET_EVENT_KEYUP,&get_key_up);
  _call_int80(INT_SET_EVENT_KEYDOWN,&get_key_down);
  for(int i = 0; i<PLAYERS;i++){
    ttinv[i] = rand() % 90;
  }
  while(1){
    while(pass < 4500000){
      pass++;
    }
    if(ch == 'x'){
      _call_int80(INT_EXIT_DRAW_MODE);
      _call_int80(INT_CLEAR);
      return;
    }
    for(int i = 0; i<PLAYERS;i++){
      tsinv[i]++;
      if(tsinv[i] == ttinv[i] + 10){
        tsinv[i] = 0;
        ttinv[i] = rand() % 90;
      }
      angle[i]+= 3*mod[i];
      accumx[i]+= 3*_cos(angle[i]);
      accumy[i]+= 3*_sin(angle[i]);

      if(!game_over[i]){
        p[i].x = accumx[i];
        p[i].y = accumy[i];
        if(tsinv[i] < ttinv[i]){
          _call_int80(INT_DRAW_CIRCLE,&p[i],RADIUS,&c[i]);
          game_over[i] = draw_into_board(i+1,p[i]);
          if(erasable[i] > 0){
            _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,erasable[i]);
            erasable[i] = -1;
          }
        }else{
          if(erasable[i] > 0){
            _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,erasable[i]);
          }
          erasable[i] = _call_int80(INT_DRAW_ERASABLE_CIRCLE,&p[i],RADIUS,&c[i]);
        }
      }
    }
    if(game_over[0] && game_over[1] && game_over[2]){
      exit_game();
      print_message("Corbata",0xFF);
      return;
    }else if(game_over[0] && game_over[1]){
      exit_game();
      print_message("Gano jugador 3",0xFF);
      return;
    }else if(game_over[0] && game_over[2]){
      exit_game();
      print_message("Gano jugador 2",0xFF);
      return;
    }else if(game_over[1] && game_over[2]){
      exit_game();
      print_message("Gano jugador 1",0xFF);
      return;
    }
    pass = 0;
  }
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  //MALLOC DE UN TABLERO??
}
void exit_game(){
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  _call_int80(INT_EXIT_DRAW_MODE);
  //_call_int80(INT_CLEAR);
}
void get_key_down(uint8_t key){
  switch(key){
    case 'a':
    mod[0] =-1;
    break;
    case 'd':
    mod[0] = 1;
    break;
    case 'j':
    mod[1] = -1;
    break;
    case 'l':
    mod[1] = 1;
    break;
    case 75:
    mod[2] = -1;
    break;
    case 77:
    mod[2] = 1;
    break;
  }
}
void get_key_up(uint8_t key){
  switch(key){
    case 'a':
    mod[0] = 0;
    break;
    case 'd':
    mod[0] = 0;
    break;
    case 'j':
    mod[1] = 0;
    break;
    case 'l':
    mod[1] = 0;
    break;
    case 75:
    mod[2] = 0;
    break;
    case 77:
    mod[2] = 0;
    break;
  }
}
char draw_into_board(uint32_t pn,Point p){
  if(p.x-RADIUS < 0 || p.x+RADIUS > 1024){
    return 1;
  }
  if(p.y-RADIUS < 0 || p.y+RADIUS > 768){
    return 1;
  }
  for(signed int y=-RADIUS ; y<=RADIUS; y++){
    for(signed int x=-RADIUS ; x<=RADIUS; x++){
      if(x*x+y*y <= RADIUS*RADIUS){
        if(board[p.x + x + (p.y+y)*1024 ] == last[pn-1] || board[p.x + x + (p.y +y)*1024] == 0){
          board[p.x + x + (p.y +y)*1024 ] = next[pn-1];
        }else{
          print_message("Crash at",0xFF);
          print_number(p.x + x);
          print_message(",",0xFF);
          print_number(p.y +y);
          print_message("Extra",0xFF);
          print_number(board[p.x + x + (p.y +y)*1024]);
          print_number(pn);
          print_number(next[pn-1]);
          print_number(last[pn-1]);

          return 1;
        }
      }
    }
  }
  uint32_t aux;
  aux = last[pn - 1];
  last[pn - 1] = next[pn - 1];
  next[pn - 1] = aux;
  return 0;
}
