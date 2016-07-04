#include "sounds.h"
#include <stdint.h>

void play_sound(uint32_t nFrequence); 

static uint8_t * const songsDirections[2] = {BECAUSE,RICKROLL};

extern void outb(uint8_t, uint8_t);
extern uint8_t inb(uint8_t);
extern void _song_note(uint32_t frec, uint32_t length);

extern void _int_start_sound(uint32_t nFrequence);
extern void _int_end_sound();

uint32_t notes[26] = {11492, 10847, 9121, 8609, 8126, 7670, 7239, 6833, 6449, 6087,
					  5743, 5423, 5119, 4831, 4560, 4304, 4063, 3834, 3619, 3416,
					  3224, 3043, 2873, 2711, 2559, 2416};

void init_songs(){

}

void read_song(uint64_t songNum){
	if(songNum == 1){
		print_message("   ////\\     //||\\     //\\|\\      ///||\\\n",0xFF);
 		print_message(" /`O-O'     ` @ @\\     //o o//       a a\n",0xFF);
 		print_message("    ]          >        ) | (         _)\n",0xFF);
 		print_message("    -          -          -           ~\n",0xFF);
 		print_message("  John        Paul      George      Ringo\n",0xFF);
	
		play_song2(0,1200);
	}
	if(songNum == 2){
		play_song2();
	}
}

void play_song2(){
	int size = 26, fd=0;
	char buf[2];
	int end = 0;
	int growing = 1, i=0;;
	while((fd=openfifo("song")) == 0);
	while(!end){
		if(i==-1){
			i=0;
			growing = 1;
		}else if(i==size){
			i=size-1;
			growing = 0;
		}
		_int_start_sound(notes[i]);
		add_new_sleep(get_current_process(), 100);
		_int_end_sound();
		if(growing){
			i++;
		}else{
			i--;
		}
		end = readfifo(fd,buf,2);
	}
	closefifo(fd);
}

 
void playPiano(uint32_t frec){
		if(frec == 0){
			_int_end_sound();
		}else{
			_int_start_sound(frec);
		}
}
