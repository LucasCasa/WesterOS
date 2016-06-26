#include "include/typedef.h"

#include "graphic_manager.h"
#include "sounds.h"
#include "allocator.h"
#include "sys_lib.h"
#include "rtc.h"

IDT_entry* IDT;
MemoryMap mm;

extern void _sti();
extern void _write_port(char port,char value);
extern void _int_timer_hand();
extern void setCurrentVideo(uint8_t* a);
extern void _int_keyboard_hand();
extern int _int80_hand();
extern void _int_start_sound();
extern void _set_graphics();
extern void _change_mode();
extern int initIPC();
extern void init_PIT();

void init_serial();

void setup_IDT_entry (int index,uint16_t selector, uint64_t offset);
void set_interrupts();

int kmain(){
	IDT = 0;
	set_interrupts();
	init_serial();
	init_malloc();
	set_graphic_pointer();
	initIPC();
	init_PIT(1193182 / 100);
	srand(RTCparameters(0));
	return 0;
}

void init_serial() {
	int PORT = 0x3f8;
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
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
