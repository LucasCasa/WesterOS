#include <stdint.h>
#include "include/handlers.h"
#include "sounds.h"

static int time = 0;
uint16_t sleep_time = 0;
uint32_t screensaver_time = 60 * 1000 / 55;
static int piano = 0;


void timer_handler(){
	time++;
	sleep_time++;
	if(sleep_time >= screensaver_time){
		if(sleep_time < screensaver_time + 10){
			save_screen();
			sleep_time = screensaver_time + 10;
		}
		if((sleep_time % 10) == 0){
			show_screensaver();
		}
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
 
void keyboard_handler(uint32_t scancode){
	if(scancode != 250){
		if(sleep_time >= screensaver_time){
			restore_screen();
		}else{
			if(piano == 1){
				uint32_t frec = getFrec(scancode);
				if(frec == 8){
					sys_write('\n',0xFF);
					piano = 0;
					}
				playPiano(frec);
			}
			else if(check_special_key(scancode)){
				if(keyboard_set_key(scancode_to_char(scancode)))
					sys_write(scancode_to_char(scancode),0xFF);
			}
		}
		sleep_time = 0;
	}
}
