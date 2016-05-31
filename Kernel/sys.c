#include <stdint.h>
#include "sys.h"

//syscall 1 ---> escribo en pantalla
//syscall 2 ---> borro caracter
//syscall 3 ---> devuelvo el ultimo caracter
//syscall 4 ---> modifica el modificador del video
//syscall 5 ---> clear screen
// syscall 6 ---> palabra que se escribio.
//syscall 7 ---> beep
//syscall 8 --> setea el piano
// sysall 9 --> lee la cancion que se pasó por pararametro
extern void _beep();
extern void _int_start_sound();
//extern void _int_piano_hand();

char sys_manager(int order,uint64_t arg1, uint64_t arg2){
	char c;
	switch(order){
		case WRITE:
			sys_write((char) arg1,(char) arg2);
			break;
		case ERASE_SCR:
			erase_screen();
			reset_current_video();
			break;
		case GET_STR:
			return read((char *)arg1,(uint8_t) arg2);
			break;
		case GET_CHAR:
			c = read_char();
			return c;
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
		case SCR_TIME:
			return validateScreenTime(arg1);
			break;
		case BEEP:
		 	//beep();
		 	_beep();
			break;
		case PIANO:
			setPiano();
			break;
		case SONGS:
			read_song(arg1);
			break;

	}
	return 0;
}
char read_char(){
	if(C_is_empty())
		return 0;
	char c = clean_get_char();
	return c;
}
char read(char* buff, uint8_t size){
	int i;
	if(C_is_empty())
		return 0;


	if(size == 0)
		size = 255;

	char c;
	for( i= 0; i<size && (c = clean_get_char()) != '\n';i++){
		buff[i] = c;
	}
	return i;
}
