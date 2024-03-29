#include <stdint.h>
#include "sys.h"
#include "process.h"
//syscall 1 ---> escribo en pantalla
//syscall 2 ---> borro caracter
//syscall 3 ---> devuelvo el ultimo caracter
//syscall 4 ---> modifica el modificador del video
//syscall 5 ---> clear screen
// syscall 6 ---> palabra que se escribio.
//syscall 7 ---> beep
//syscall 8 --> setea el piano
// sysall 9 --> lee la cancion que se pasó por pararametro
// syscall 10 --> lee cuantos bytes hay que malloquear
// syscall 11 --> lee que address hay que hacer free
extern void _beep();
extern void _int_start_sound();
extern void (*up)(uint8_t);
extern void (*down)(uint8_t);
int draw_mode = 0;
char char_buffer = 0;
//extern void _int_piano_hand();

uint64_t sys_manager(int order,uint64_t arg1, uint64_t arg2,uint64_t arg3){
	char c;
	switch(order){
		case WRITE:
			if(!draw_mode){
				sys_write((char) arg1,(char) arg2);
			}
			break;
		case ERASE_SCR:
			erase_screen();
			reset_current_video();
			clear_screen();
			reset_current_video_graphic();
			break;
		case GET_STR:
			return read((char *)arg1,(uint8_t) arg2);
			break;
		case GET_CHAR:
			c = read_char();
			return c;
			break;
			case GET_DIRTY_STR:
				return get_dirty_string(arg1,arg2);
			break;
		case RTC_READ:
			return RTCparameters((char)arg1);
			break;
		case RTC_WRITE:
			set_date((char)arg1,(uint32_t)arg2); // 1st arg type to change (Year, Month, Hour, etc.) 2nd arg the amount
			break;
		case COLORS:
			set_default_modifiers((uint8_t) arg1, (uint8_t) arg2);
			break;
		case COLORS_GRAPHIC:
			set_default_modifiers_graphic((Color*) arg1, (Color*) arg2,(Color*) arg3);
			break;
		case SCR_TIME:
			return validateScreenTime(arg1);
			break;
			case SET_SCREENSAVER:
			set_screensaver_image((ScreenImage *)arg1);
			break;
		case BEEP:
		 	//beep();
		 	_beep();
			break;
		case PIANO:
			setPiano();
			break;
		case SONGS:
			// read_song(arg1);
			break;
		case MALLOC:
			return (uint64_t)malloc(arg1);
			break;
		case FREE:
			free((void *)arg1);
			break;
		case ENTER_DRAW_MODE:
			draw_mode = 1;
			set_graphic_pointer();
		break;
		case EXIT_DRAW_MODE:
			draw_mode = 0;
			flush_buffer();
		break;
		case CLEAR:
			clear_screen();
			break;
		case DRAW_CIRCLE:
			draw_circle((Point*) arg1,arg2,(Color*)arg3);
		break;
		case DRAW_TEXT:
			draw_text((Point*) arg1, (char*) arg2);
		break;
		case DRAW_ERASABLE_CIRCLE:
			return draw_erasable_circle((Point*) arg1,arg2,(Color*)arg3);
		break;
		case UNDRAW_ERASABLE_CIRCLE:
			undraw_erasable_circle(arg1);
		break;
		case GET_CHAR_FROM_BUFFER:
			return get_char_from_buffer();
		break;
		case MKFIFO:
			return mkfifo((char*)arg1);
		break;
		case OPENFIFO:
			return openfifo((char*)arg1);
		break;
		case CLOSEFIFO:
			return closefifo(arg1);
		break;
		case WRITEFIFO:
			return writefifo(arg1,(void*)arg2,arg3);
		break;
		case READFIFO:
			return readfifo(arg1,(void*)arg2,arg3);
		break;
		case READFIFOBLOQ:
				return readBloq(arg1,(void*)arg2,arg3);
		break;
		case SHOWIPCS:
			list_ipc();
		break;
		case SET_EVENT_KEYUP:
			up = arg1;
		break;
		case UNSET_EVENT_KEYUP:
			up = 0;
		break;
		case SET_EVENT_KEYDOWN:
			down = arg1;
		break;
		case UNSET_EVENT_KEYDOWN:
			down = 0;
		break;
		case NEW_PROCESS:
			return add_new_process(create_process((char*) arg1,(entry) arg2,69));
		break;
		case END_PROCESS:
			remove_process(arg1);
		break;
		case GET_ALL_PROCESS:
			print_all_process();
		break;
		case SLEEP:
			add_new_sleep(get_current_process(),(int)arg1);
			//_reschedule_int();
				//print_message("HHH",0xFF);
			//while(1);
		break;
	}
	return 0;
}
char get_char_from_buffer(){
	char aux = char_buffer;
	char_buffer = 0;
		return aux;
}
char read_char(){
	if(C_is_empty()){
		block_key_clean(get_current_process()->pid);
		//return 0;
	}
	char c = clean_get_char();
	return c;
}
int get_dirty_string(char* buff,int size){
	if(keyboard_is_empty()){
		block_key();
	}
	return get_dirty(buff,size);
}
char read(char* buff, uint8_t size){
	int i;
	if(C_is_empty())
		block_key_clean(get_current_process()->pid);


	if(size == 0)
		size = 255;

	char c;
	for( i= 0; i<size && (c = clean_get_char()) != '\n';i++){
		buff[i] = c;
	}
	return i;
}
