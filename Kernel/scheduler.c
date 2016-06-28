#include "scheduler.h"

ProcessHolder phs[MAX_PROC];

ProcessHolder* current = 0;
int count = 0;
ProcessHolder* last;
extern void** stacks;
void init_scheduler(){

   uint64_t stack_end = STACK_END;

   	for(int i = 0; i<MAX_PROC; i++)
   		stacks[i] = (void*)(stack_end - STACK_SIZE*i);

}
void yield(){

}

void* next_process(){


}
void start_process(){

}
void schedule(){
    _cli();
    if(count > 110 || current->p->state == PROC_WAITING){
      count = 0;
	do{
		current = current->next;
   }while(current->p->state == PROC_WAITING);
   print_message("Cambiando a: ",0xFF);
   print_number(current->p->stack);
}else{
   count++;
}
    _sti();
}
void add_new_process(Process* p){
  _cli();
  ProcessHolder* ph = &phs[p->pid];
  ph->p = p;
  if(current == 0){
    current = ph;
    ph->next = ph;
  }else{
    ph->next = current->next;
    current->next = ph;
  }
  _sti();
}

void remove_process(uint8_t pid){

}

void push_to_top(uint8_t pid){

}

void* get_all_process(){

}

Process* get_current_process(){
	return current->p;
}

void* get_entry_point(){
   return current->p->entry_point;
}
void* switch_user_to_kernel(uint64_t esp) {
   _cli();
	current->p->stack = esp;
   _sti();
	return current->p->kernel_stack;
}

void* switch_kernel_to_user(uint64_t esp){
	schedule();
	return current->p->stack;
}
