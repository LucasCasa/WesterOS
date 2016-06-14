#include "scheduler.h"

ProcessHolder* current = 0;
ProcessHolder* last;
void init_scheduler(){


}

void* next_process(){


}

void add_new_process(Process* p){
ProcessHolder ph;
ph.p = p;
ph.next = current->next;
current->next = &ph;

}

void remove_process(uint8_t pid){

}

void push_to_top(uint8_t pid){

}

void* get_all_process(){

}
