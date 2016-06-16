#include "scheduler.h"

stack_ptr stacks[64];
task_t tasks[64];

ProcessHolder* current = 0;
ProcessHolder* last;
void init_scheduler(){


}

void* next_process(){


}
void start_process(){

}
void schedule(){

}
void add_new_process(Process* p){
  _cli();
  ProcessHolder ph;
  ph.p = p;
  if(current == 0){
    current = &ph;
  }else{
    ph.next = current->next;
    current->next = &ph;
  }
  _sti();
}

void remove_process(uint8_t pid){

}

void push_to_top(uint8_t pid){

}

void* get_all_process(){

}
