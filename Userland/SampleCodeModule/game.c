#include "lib.h"
#define  RADIUS 10
extern double _sin(int angle);
extern double _cos(int angle);
char board[1024*768];
void get_key_up(uint8_t);
void get_key_down(uint8_t);
int mod1 = 0,mod2 = 0;
void game(){
   //CANTIDAD DE JUGADORES
   Point p1 = {20,20};
   Color c1 = {255,0,0};

   Point p2 = { 1000,700};
   Color c2 = {0,0,255};
   double accumx1 = p1.x, accumy1 = p1.y;
   double accumx2 = p2.x, accumy2 = p2.y;

   _call_int80(INT_ENTER_DRAW_MODE);
   _call_int80(INT_CLEAR);

   int pass = 0;
   int ch = 0;

   int angle1 = 0;
   int angle2 = 180;

   _call_int80(INT_SET_EVENT_KEYUP,&get_key_up);
   _call_int80(INT_SET_EVENT_KEYDOWN,&get_key_down);

   while(1){
      while(pass < 3000000){
         pass++;
      }
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
   angle1+= 3*mod1;
   angle2+= 3*mod2;
   accumx1+= 2*_cos(angle1);
   accumy1+= 2*_sin(angle1);
   accumx2+= 2*_cos(angle2);
   accumy2+= 2*_sin(angle2);
   ch = 0;
   pass = 0;


   p1.x = accumx1;
   p1.y = accumy1;
   _call_int80(INT_DRAW_CIRCLE,&p1,RADIUS,&c1);

   p2.x = accumx2;
   p2.y = accumy2;
   _call_int80(INT_DRAW_CIRCLE,&p2,RADIUS,&c2);

   }
   _call_int80(INT_UNSET_EVENT_KEYUP);
   _call_int80(INT_UNSET_EVENT_KEYDOWN);
   //MALLOC DE UN TABLERO??
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
