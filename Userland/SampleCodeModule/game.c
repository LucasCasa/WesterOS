#include "lib.h"
#define  RADIUS 5
extern double _sin(int angle);
extern double _cos(int angle);
char board[1024*768];
void get_key_up(uint8_t);
void get_key_down(uint8_t);
int mod1 = 0,mod2 = 0;
uint32_t last[] = {1,2};
uint32_t next[] = {3,4};
int ttinv1 = 255;
int ttinv2 = 255;
int tsinv1 = 0;
int tsinv2 = 0;

char draw_into_board(uint32_t pn,Point p);
void exit_game();

void game(){
   //CANTIDAD DE JUGADORES
   srand(*((uint32_t*)0xB8078));
   mod1 = 0;
   mod2 = 0;
   Point p1 = {20,20};
   Color c1 = {255,0,0};
   Color c1h = {100,0,0};
   for(int i = 0; i<1024*768;i++){
     board[i] = 0;
   }
   Point p2 = { 1000,700};
   Color c2 = {0,0,255};
   Color c2h = {0,0,100};
   double accumx1 = p1.x, accumy1 = p1.y;
   double accumx2 = p2.x, accumy2 = p2.y;

   _call_int80(INT_ENTER_DRAW_MODE);
   _call_int80(INT_CLEAR);
   //_call_int80(INT_EXIT_DRAW_MODE);
   char game_over1 = 0;
   char game_over2 = 0;

   int pass = 0;
   int ch = 0;

   int angle1 = 0;
   int angle2 = 180;

   _call_int80(INT_SET_EVENT_KEYUP,&get_key_up);
   _call_int80(INT_SET_EVENT_KEYDOWN,&get_key_down);
   ttinv1 = rand() % 90;
   ttinv2 = rand() % 90;
    while(1){
      while(pass < 6000000){
         pass++;
      }
    //_call_int80(INT_DRAW_CIRCLE,&p1,RADIUS,&c1h);
    //_call_int80(INT_DRAW_CIRCLE,&p2,RADIUS,&c2h);
/*
      if(ch == 'a'){
         angle1-=10;
      }else if(ch == 'd'){
         angle1+=10;
      }else if(ch == 'k'){
         angle2+=10;
      }else if(ch == 'l'){
         angle2+=10;
      }*/
   if(ch == 'x'){
      _call_int80(INT_EXIT_DRAW_MODE);
      _call_int80(INT_CLEAR);
      return;
   }
   tsinv1++;
   if(tsinv1 == ttinv1 + 10){
     tsinv1 = 0;
     ttinv1 = rand() % 90;
   }
   tsinv2++;
   if(tsinv2 == ttinv2 + 10){
     tsinv2 = 0;
     ttinv2 = rand() % 90;
   }
   angle1+= 3*mod1;
   angle2+= 3*mod2;
   accumx1+= 3*_cos(angle1);
   accumy1+= 3*_sin(angle1);
   accumx2+= 3*_cos(angle2);
   accumy2+= 3*_sin(angle2);
   ch = 0;
   pass = 0;

   p1.x = accumx1;
   p1.y = accumy1;
   if(tsinv1 < ttinv1){
   _call_int80(INT_DRAW_CIRCLE,&p1,RADIUS,&c1);
   game_over1 = draw_into_board(1,p1);
  }
  p2.x = accumx2;
  p2.y = accumy2;
  if(tsinv2 < ttinv2){
    _call_int80(INT_DRAW_CIRCLE,&p2,RADIUS,&c2);
   game_over2 = draw_into_board(2,p2);
  }

   if(game_over1 && game_over2){
     exit_game();
     print_message("Corbata",0xFF);
     return;
   }else if(game_over1){
     exit_game();
     print_message("Gano jugador 2",0xFF);
     return;
   }else if(game_over2){
     exit_game();
     print_message("Gano jugador 1",0xFF);
     return;
   }
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
         mod1 =-1;
      break;
      case 'd':
         mod1 = 1;
      break;
      case 'j':
         mod2 = -1;
      break;
      case 'k':
         mod2 = 1;
      break;
   }
}
void get_key_up(uint8_t key){
   switch(key){
      case 'a':
         mod1 = 0;
      break;
      case 'd':
         mod1 = 0;
      break;
      case 'j':
         mod2 = 0;
      break;
      case 'k':
         mod2 = 0;
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
