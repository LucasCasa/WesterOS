#include "lib.h"
#define RADIUS      4
#define MAX_PLAYERS 6 // DOBLE TECLADO SE PUEDEN HASTA 6??
#define HOLE_SIZE   10
#define MAX_DRAW    90
#define COLOR_POS   50
#define NUMBER_POS  180
#define RIGHT_POS   310
#define LEFT_POS    500
extern double _sin(int angle);
extern double _cos(int angle);
char board[1024*768];
void get_key_up(uint8_t);
void get_key_down(uint8_t);
int mod[MAX_PLAYERS] = {0,0,0,0,0,0};
uint32_t last[] = {1,2,3,4,5,6};
uint32_t next[] = {7,8,9,11,12};
int ttinv[MAX_PLAYERS] = {0,0,0,0,0,0};
int tsinv[MAX_PLAYERS] = {0,0,0,0,0,0};
int erasable[MAX_PLAYERS] = {-1,-1,-1,-1,-1,-1};
Color c[] = {{255,0,0},{0,0,255},{0,255,0},{255,0,255},{255,255,0},{255,255,255}};
char* controls[6][2] = {{"A","D"},{"J","L"},{"L Arrow","R Arrow"},{"Z","C"},{"I","P"},{"1","3"}};
int starting = 1;
char draw_into_board(uint32_t pn,Point p);
void exit_game();

void game(){
  _call_int80(INT_ENTER_DRAW_MODE);
  _call_int80(INT_CLEAR);

  //CANTIDAD DE JUGADORES
  int nplayers = lobby();
  _call_int80(INT_CLEAR);
  starting = 1;
  Point p[MAX_PLAYERS];
  int angle[MAX_PLAYERS];
  for(int i = 0; i<nplayers;i++){
     p[i].x = rand() % 600 + 100;
     p[i].y = rand() % 500 + 80;
     angle[i] = rand() % 360;
 }

  //Color ch[] = {{100,0,0},{0,0,100},{0,100,0},{100,0,100}};
  for(int i = 0; i<1024*768;i++){
    board[i] = 0;
  }
  double accumx[MAX_PLAYERS];
  double accumy[MAX_PLAYERS];
  for(int i = 0; i<nplayers;i++){
    accumx[i] = p[i].x;
    accumy[i] = p[i].y;
    erasable[i] = -1;
    mod[i] = 0;
    ttinv[i] = 0;
    tsinv[i] = 0;
  }


  char game_over[MAX_PLAYERS] = {0,0,0,0,0,0};

  int pass = 0;




  _call_int80(INT_SET_EVENT_KEYUP,&get_key_up);
  _call_int80(INT_SET_EVENT_KEYDOWN,&get_key_down);


  while(1){
    while(pass < 4500000){
      pass++;
    }

    for(int i = 0; i<nplayers;i++){
      if(!starting){
         tsinv[i]++;
      }
      if(tsinv[i] == ttinv[i] + HOLE_SIZE){
        tsinv[i] = 0;
        ttinv[i] = rand() % MAX_DRAW;
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
    int alive = 0;
    for(int i = 0; i<nplayers;i++){
      alive+= game_over[i];
    }
    if(alive >= nplayers -1){
      for(int i = 0; i<nplayers;i++){
        if(!game_over[i]){
          exit_game();
          print_message("Gano Jugador ",0xFF);
          print_number(i+1);
          print_message("\n",0xFF);
          return;
        }
      }
      exit_game();
      print_message("Corbata",0xFF);
      return;
    }
    pass = 0;
    if(starting == 40){
      starting = 0;
    }else if( starting > 0){
      starting++;
    }
  }
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  //MALLOC DE UN TABLERO??
}
int lobby(){
  char* div = "|";
  char* bar = "_________________________________";
  char play[2] = "1";
  Point p = {50,50};
    _call_int80(INT_DRAW_TEXT,&p,"Ingresar cantidad de jugadores(2 a 6):");
    p.y+=50;
    p.x = COLOR_POS;
    _call_int80(INT_DRAW_TEXT,&p,"COLOR");
    p.x = NUMBER_POS - 20;
    _call_int80(INT_DRAW_TEXT,&p,div);
    p.x = NUMBER_POS;
    _call_int80(INT_DRAW_TEXT,&p,"NUMERO");
    p.x = RIGHT_POS- 20;
    _call_int80(INT_DRAW_TEXT,&p,div);
    p.x = RIGHT_POS;
    _call_int80(INT_DRAW_TEXT,&p,"IZQUIERDA");
    p.x = LEFT_POS- 20;
    _call_int80(INT_DRAW_TEXT,&p,div);
    p.x = LEFT_POS;
    _call_int80(INT_DRAW_TEXT,&p,"DERECHA");
    p.y+=25;
    p.x = COLOR_POS;
    _call_int80(INT_DRAW_TEXT,&p,bar);
    p.y+=30;
    for(int i = 0; i<6;i++){
      p.y+= 10;
      p.x = COLOR_POS + ((NUMBER_POS - COLOR_POS) / 2 - RADIUS*2);
      _call_int80(INT_DRAW_CIRCLE,&p,RADIUS*2,&c[i]);
      p.y-=10;
      p.x = NUMBER_POS - 20;
      _call_int80(INT_DRAW_TEXT,&p,div);
      p.x = NUMBER_POS + ((RIGHT_POS - NUMBER_POS) / 2 - 11);
      play[0] = i + '1';
      _call_int80(INT_DRAW_TEXT,&p,play);
      p.x = RIGHT_POS- 20;
      _call_int80(INT_DRAW_TEXT,&p,div);
      p.x = RIGHT_POS;
      _call_int80(INT_DRAW_TEXT,&p,controls[i][0]);
      p.x = LEFT_POS - 20;
      _call_int80(INT_DRAW_TEXT,&p,div);
      p.x= LEFT_POS;
      _call_int80(INT_DRAW_TEXT,&p,controls[i][1]);
      p.y+=30;
    }
    /*
    _call_int80(INT_DRAW_TEXT,&p,"Jugador 2");
    _call_int80(INT_DRAW_TEXT,&p,"Jugador 3");
    _call_int80(INT_DRAW_TEXT,&p,"Jugador 4");
    _call_int80(INT_DRAW_TEXT,&p,"Jugador 5");
    _call_int80(INT_DRAW_TEXT,&p,"Jugador 6");
    */
    char cha = 0;
    do{
      cha = _call_int80(INT_GCFB);
    }while(cha < '2' || cha > '6');
      return cha-'0';
}
void exit_game(){
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  _call_int80(INT_EXIT_DRAW_MODE);
  _call_int80(INT_ERASE_SCR);
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
    case '4':
    mod[2] = -1;
    break;
    case '6':
    mod[2] = 1;
    break;
    case 'z':
    mod[3] = -1;
    break;
    case 'c':
    mod[3] = 1;
    break;
    case 'i':
    mod[4] = -1;
    break;
    case 'p':
    mod[4] = 1;
    break;
    case '1':
    mod[5] = -1;
    break;
    case '3':
    mod[5] = 1;
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
    case '4':
    mod[2] = 0;
    break;
    case '6':
    mod[2] = 0;
    break;
    case 'z':
    mod[3] = 0;
    break;
    case 'c':
    mod[3] = 0;
    break;
    case 'i':
    mod[4] = 0;
    break;
    case 'p':
    mod[4] = 0;
    break;
    case '1':
    mod[5] = 0;
    break;
    case '3':
    mod[5] = 0;
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
