#include "scheduler.h"

ProcessHolder phs[MAX_PROC];
uint8_t keypid[MAX_PROC]; // dirty_keyboard
uint8_t ckeypid[MAX_PROC]; // clean_keyboard
ProcessHolder* current = 0;
int count = 0;
ProcessHolder* last;
extern void** stacks;
uint8_t get_next_block_slot(uint8_t* buf);
void init_scheduler(){

   uint64_t stack_end = STACK_END;

   	for(int i = 0; i<MAX_PROC; i++){
      keypid[i] = 0;
      ckeypid[i] = 0;
    }

}
void yield(){

}

void* next_process(){


}
void start_process(){

}
void schedule(){
    _cli();
    if(count > 1 || current->p->state == PROC_WAITING || current->p->state == PROC_FINISHED ){
      count = 0;
	do{
		current = current->next;
   }while(current->p->state == PROC_WAITING || current->p->state == PROC_FINISHED);
   //print_message(current->p->name,0xFF);
   //      while(1);
}else{
   count++;
}
    _sti();
}
int add_new_process(Process* p){
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
  process_ready(p);
  _sti();
  return p->pid;
}

void remove_process(uint8_t pid){
      int removed = 1;
      ProcessHolder* aux;
      aux = current;
      if(current->next == current){
         current = 0;
      }
      while(!removed){
         if(aux->next->p->pid == pid){
            aux->next = aux->next->next;
            removed = 0;
         }else{
            aux = aux->next;
         }
      }
}

void push_to_top(uint8_t pid){
   int i = 1;
   ProcessHolder* aux;
   ProcessHolder* self;
   aux = current;
   while(!i){
      if(aux->next->p->pid == pid){
         self = aux->next;
         aux->next = aux->next->next;
         i = 0;
      }else{
         aux = aux->next;
      }
   }
   self->next = current->next;
   current->next = self;

}

void* get_all_process(){

}

Process* get_current_process(){
	return current->p;
}

void* get_entry_point(){
   return current->p->entry_point;
}
void* switch_user_to_kernel(uint64_t rsp) {
   _cli();
	current->p->stack = rsp;
   if(current->p->state != PROC_FINISHED && current->p->state != PROC_WAITING)
      current->p->state = PROC_READY;
   _sti();
	return current->p->kernel_stack;
}

void* switch_kernel_to_user(uint64_t rsp){
	schedule();
   if(current->p->state != PROC_FINISHED && current->p->state != PROC_WAITING){
      current->p->state = PROC_RUNNING;
   }else{
      print_message("Estoy scheduleando algo que no tendria\n",0xFF);
   }
	return current->p->stack;
}

void block_key(uint8_t pid){
  keypid[get_next_block_slot(keypid)] = pid;
  process_waiting();
}
void block_key_clean(uint8_t pid){

}
uint8_t get_next_block_slot(uint8_t* buf){
  int i = 0;
  //while()
}
void check_key_blocked(){
  if(!keyboard_is_empty()){
    for(int i = 0; i<MAX_PROC;i++){
      if(keypid[i] != 0){
        process_ready(keypid[i]);
      }
    }
  }
}
