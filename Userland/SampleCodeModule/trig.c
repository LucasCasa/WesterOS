#include "trig.h"

double _sin(int degree){
   int mod = 1;
   if(degree < 0){
      mod = -1;
      degree*=-1;
   }
   int v = degree % 360;
   if(v <= 90){
      return sin_table[v]*mod;
   }else if( v<= 180){
      return sin_table[180 - v]*mod;
   }else if(v <= 270){
      return -1*sin_table[v-180]*mod;
   }else{
      return -1*sin_table[360 - v]*mod;
   }
}
double _cos(int degree){
   return _sin(90 - degree);
}
double _tan(int degree){
   if(degree% 90 == 0){
      return 0;
   }
   return _sin(degree)/_cos(degree);
}
