#include "include/typedef.h"

#include "sounds.h"


IDT_entry* IDT;

extern void _sti();
extern void _write_port(char port,char value);
extern void _int_timer_hand();
extern void _int_keyboard_hand();
extern int _int80_hand();
extern void _int_start_sound();




void setup_IDT_entry (int index,uint16_t selector, uint64_t offset);
void set_interrupts();

int kmain(){
	IDT = 0;
	set_interrupts();
	return 0;
}

void set_interrupts() {	
	setup_IDT_entry(0x20,0x08,(uint64_t) &_int_timer_hand);
	setup_IDT_entry(0x21,0x08,(uint64_t) &_int_keyboard_hand);
	setup_IDT_entry(0x80,0x08,(uint64_t) &_int80_hand);
	setup_IDT_entry(0x61,0x08,(uint64_t) &_int_start_sound);
	_sti();
	_write_port(0x21,0xFC);
}

void setup_IDT_entry(int index,uint16_t selector, uint64_t offset){
 	IDT[index].selector = selector;
	IDT[index].offset_l = offset & 0xFFFF;
	IDT[index].offset_m = (offset & 0xFFFF0000) >> 16;
	IDT[index].offset_h = (offset & 0xFFFFFFFF00000000)>> 32;
	IDT[index].type_attr = 0x8E;
	IDT[index].zero = 0;
	IDT[index].zero2 = 0;
}
