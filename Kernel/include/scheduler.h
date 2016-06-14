#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "process.h"

typedef struct ProcessHolder{
  struct Process* p;
  struct ProcessHolder* next;
}ProcessHolder;

void* next_process();
void init_scheduler();
void remove_process(uint8_t pid);
void push_to_top(uint8_t pid);
void* get_all_process();


#endif
