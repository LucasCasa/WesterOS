#include "process.h"
void* stacks[MAX_PROC];
Process processes[MAX_PROC];
uint8_t next_pid = 1;
void process_wrapper(entry);
void * to_stack_address(void * page);

void* fill_stack_frame(void * entry_point, void * user_stack) {
	stack_frame* frame = (stack_frame*) user_stack -1;
	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	(uint64_t) entry_point;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	(uint64_t)process_wrapper;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return frame;
}
Process* create_process(char*name,entry entry_point){
	//_cli();
	Process* p = &processes[next_pid]; //ESTO CON UN MALLOC
	p->pid = next_pid++;
	int i = 0;
	while(name[i] != 0){
		p->name[i] = name[i];
		i++;
	}
	p->name[i] = 0;

	//put_char('a',0xFF);
	p->kernel_stack = malloc(1);
	//print_number(p->kernel_stack);
	//put_char('b',0xFF);
	p->stack = malloc(1);
	//put_char('c',0xFF);
	p->stack = to_stack_address(p->stack);
	//put_char('d',0xFF);
	p->kernel_stack = to_stack_address(p->kernel_stack);
	p->stack = fill_stack_frame(entry_point,p->stack);
	p->entry_point = entry_point;
	process_out_of_scheduler(p);
	//_sti();
	return p;

}
void * to_stack_address(void * page) {
	return (uint8_t*)page + 4098 - 0x10;
}
void process_wrapper(entry proc) {
	 proc(0);
	 _cli();
	 print_message("Proceso Terminado \n",0xFF);
    Process* p = get_current_process();
	 process_finished(p);
    remove_process(p->pid);
	 // while(1);
	 _sti();
    _reschedule();
	 while(1);
}
void process_ready(Process* p){
	p->state = PROC_READY;
}
void process_waiting(Process* p){
	p->state = PROC_WAITING;
	_reschedule_int();
}
void process_finished(Process *p){
	p->state = PROC_FINISHED;
}
void process_out_of_scheduler(Process *p){
	p->state = PROC_OUT_OF_SCHEDULER;
}

void print_all_process(){
	print_message("\n\n***************************************************\n",0xFF);
	print_message("******* List of currently executing Process *******\n",0xFF);
	print_message("***************************************************\n\n",0xFF);
	print_message("Process: ",0xFF);
	print_message("\tPID: ");
	print_message("\tState: \n",0xFF);
	for(int i=0; i<next_pid; i++){
		if(processes[i].state != PROC_FINISHED && processes[i].state != 0){
			print_process(&processes[i]);
		}
	}
}
void print_process(Process *p){
	print_message(p->name,0xFF);
	print_message("\t",0xFF);
	print_number(p->pid);
	print_message("\t",0xFF);
	switch (p->state) {
		case PROC_RUNNING:
			print_message("Running \n",0xFF);
		break;
		case PROC_READY:
			print_message("Ready \n",0xFF);
		break;
		case PROC_WAITING:
			print_message("Waiting \n",0xFF);
		break;
		case PROC_OUT_OF_SCHEDULER:
			print_message("Out of Scheduler \n",0xFF);
			break;
		default:
			print_message("Unknown state; ",0xFF);
			print_number(p->state);
			print_message("\n",0xFF);
		break;
	}
}
