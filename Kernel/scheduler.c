#include "scheduler.h"

ProcessHolder phs[MAX_PROC];
uint8_t keypid[MAX_PROC]; // dirty_keyboard
uint8_t ckeypid[MAX_PROC]; // clean_keyboard
ProcessHolder* current = 0;
ProcessHolder* ina = 0;
int count = 0;
int p_inactive = 0;
ProcessHolder* last;
extern void** stacks;
uint8_t get_next_block_slot(uint8_t* buf);
void set_foreground_process(uint8_t pid);
void init_scheduler(){

   uint64_t stack_end = STACK_END;

   	for(int i = 0; i<MAX_PROC; i++){
      keypid[i] = 0;
      ckeypid[i] = 0;
    }

}
void yield(){

}
void schedule(){
    _cli();
    if(count > 1 || current->p->state == PROC_WAITING || current->p->state == PROC_FINISHED || p_inactive){
      count = 0;
      ProcessHolder* aux = current;
      p_inactive = 0;
      int i = 0;
	do{
		current = current->next;
    if(aux == current && ina != 0 && current->p->state == PROC_WAITING){
      p_inactive = 1;
    }else{
      p_inactive = 0;
    }
  }while((current->p->state == PROC_WAITING || current->p->state == PROC_FINISHED) && !p_inactive);
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
  if(strcmp(p->name,"Snorlax")){
    set_foreground_process(p->pid);
  }
  return p->pid;
}
void add_inactive(Process * p){
    _cli();
    ProcessHolder* ph = &phs[p->pid];
    ph->p = p;
    ph->next = current;
    process_ready(p);
    ina = ph;
    return;
}
void remove_process(uint8_t pid){
      int removed = 0;
      ProcessHolder* aux;
      ProcessHolder* aux2;
      aux = current;
      if(current->next == current){
         current = 0;
      }
      if(current->p->pid == pid){
        current = current->next;
      }
      while(!removed){
         if(aux->next->p->pid == pid){
           set_foreground_process(aux->next->p->ppid);
            aux2 = aux->next;
            aux->next = aux2->next;
            removed = 1;
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
void set_foreground_process(uint8_t pid){
  if(current->p->pid == pid){
    current->p->has_foreground = 1;
  }else{
    current->p->has_foreground = 0;
  }
  ProcessHolder* aux = current->next;
  while(aux->p->pid != current->p->pid){
    if(aux->p->pid == pid){
      aux->p->has_foreground = 1;
    }else{
      aux->p->has_foreground = 0;
    }
    aux = aux->next;
  }
}

Process* get_current_process(){
	return current->p;
}

void* get_entry_point(){
   return current->p->entry_point;
}
Process* get_process(uint8_t pid){
  if(current->p->pid == pid){
    return current->p;
  }
  ProcessHolder* aux = current->next;
  while(aux->p->pid != pid){
    if(aux == current){
      return 0; //ERROR;
    }
    aux = aux->next;
  }
  return aux->p;
}
void* switch_user_to_kernel(uint64_t rsp) {
   _cli();
   if(p_inactive){
     ina->p->stack = rsp;
    _sti();
     return ina->p->kernel_stack;
   }else{
     current->p->stack = rsp;
      if(current->p->state != PROC_FINISHED && current->p->state != PROC_WAITING)
         current->p->state = PROC_READY;
   }

	return current->p->kernel_stack;
}

void* switch_kernel_to_user(uint64_t rsp){
	schedule();
   if(p_inactive){
      return ina->p->stack;
   }else{
     if(current->p->state != PROC_FINISHED && current->p->state != PROC_WAITING){
        current->p->state = PROC_RUNNING;
     }else{
        print_message("Estoy scheduleando algo que no tendria\n",0xFF);
     }
	    return current->p->stack;
  }
}

void block_key(uint8_t pid){
  keypid[get_next_block_slot(keypid)] = pid;
  process_waiting();
}
void block_key_clean(uint8_t pid){
  ckeypid[get_next_block_slot(ckeypid)] = pid;
  process_waiting(get_process(pid));
}
uint8_t get_next_block_slot(uint8_t* buf){
  int i = 0;
  while(buf[i] != 0){
    i++;
  }
  return i;
}
void check_key_blocked(){
  if(!keyboard_is_empty()){
    for(int i = 0; i<MAX_PROC;i++){
      if(keypid[i] != 0){
        process_ready(get_process(keypid[i]));
        keypid[i] = 0;
      }
    }
  }
}
void check_clear_buffer(){
    for(int i = 0; i<MAX_PROC;i++){
      if(ckeypid[i] != 0 && get_process(ckeypid[i])->has_foreground == 1){
        print_message("Libero el proceso: ",0xFF);
        print_message(get_process(ckeypid[i])->name,0xFF);
        process_ready(get_process(ckeypid[i]));
        ckeypid[i] = 0;
      }
    }
}
