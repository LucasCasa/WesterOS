#include "process.h"
void* stacks[MAX_PROC];
Process processes[MAX_PROC];
uint8_t next_pid = 1;
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
	frame->rdi =	0x00C;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	(uint64_t)entry_point;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return frame;
}
Process* create_process(entry entry_point){
	//_cli();
	Process* p = &processes[next_pid]; //ESTO CON UN MALLOC
	put_char('a',0xFF);
	p->kernel_stack = malloc(1);
	print_number(p->kernel_stack);
	put_char('b',0xFF);
	p->stack = malloc(1);
	put_char('c',0xFF);
	p->stack = to_stack_address(p->stack);
	put_char('d',0xFF);
	p->kernel_stack = to_stack_address(p->kernel_stack);
	fill_stack_frame(entry_point,p->stack);
	p->entry_point = entry_point;
	process_ready(p);
	//_sti();
	return p;

}
void * to_stack_address(void * page) {
	return (uint8_t*)page + 4098 - 0x10;
}
void process_wrapper(entry proc) {

	 proc(0);

    Process* p = get_current_process();
    remove_process(p->pid);
    //_reschedule();
}
void process_ready(Process* p){
	p->state = PROC_READY;
}
void process_waiting(Process* p){
	p->state = PROC_WAITING;
}
