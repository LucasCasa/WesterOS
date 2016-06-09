#include <stdint.h>
#include "include/handlers.h"
#include "sounds.h"

static int time = 0;
uint16_t sleep_time = 0;
uint32_t screensaver_time = 10 * 1000 / 55;
static int piano = 0;
extern char_buffer; // ESTO SE TEDNRIA QUE HACER BIEN...
void (*up)(uint8_t) = 0;
void (*down)(uint8_t) = 0;

void timer_handler(){
	time++;
	sleep_time++;
	if(sleep_time >= screensaver_time){
		if(sleep_time < screensaver_time + 10){
			save_screen();
			save_screen_graphic();
			clear_screen();
			sleep_time = screensaver_time + 10;
		}
			show_screensaver();
	}else{
		if(time >= 10){
			print_standby();
			time = 0;
		}
	}
}

void setPiano(){
	piano =1;
}

void keyboard_handler(uint8_t scancode){
	if(scancode > 128 && up != 0){
		uint8_t axu = scancode & 0x7F;
		up(scancode_to_char(axu)); //UP
	}
	if(scancode != 250){
		if(sleep_time >= screensaver_time){
			restore_screen();
			restore_screen_graphic();
		}else{
			if(piano == 1){
				uint32_t frec = getFrec(scancode);
				if(frec == 8){
					sys_write('\n',0xFF);
					piano = 0;
					}
				playPiano(frec);
			}else if(check_special_key(scancode)){
				if(scancode < 128 && down != 0){
					down(scancode_to_char(scancode));
				}
				if(keyboard_set_key(scancode_to_char(scancode)))
					if(!draw_mode){
						sys_write(scancode_to_char(scancode),0xFF);
					}
					char_buffer = scancode_to_char(scancode);
			}
		}
		sleep_time = 0;
	}
}
