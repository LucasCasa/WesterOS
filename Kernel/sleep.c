#include "sleep.h"


slp sleeping[MAX_SLEEP]; // max sleeping simultaneously
int next_slot = 0;

void init_sleep(){
      for(int i = 0; i<MAX_SLEEP;i++){
         sleeping[i].p = 0;
      }
}

void add_new_sleep(Process * p, int msec){
   do{
      if(next_slot == MAX_SLEEP){
         next_slot = 0;
      }else{
         next_slot++;
      }
   }while(sleeping[next_slot].p != 0);
   sleeping[next_slot].p = p;
   sleeping[next_slot].msec = msec;
   print_message("sleeping \n",0xFF);
   process_waiting(p);
}

void check_sleeping(){
   for(int i = 0; i<MAX_SLEEP;i++){
      if(sleeping[i].p != 0){
         sleeping[i].msec-= 1000 / PIT_FREQ;
         if(sleeping[i].msec <= 0){
            wake_up(sleeping[i].p);
            print_message("waking \n",0xFF);
            sleeping[i].p = 0;
         }
      }
   }
}

void wake_up(Process *p){
   process_ready(p);
   //push to top???
}
