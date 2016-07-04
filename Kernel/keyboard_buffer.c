#include <stdint.h>
#include "include/keyboard_buffer.h"
//El Buffer de Teclado guarda Scan Codes
uint8_t keyboard_buffer[KB_SIZE];

// Indices para iterar sobre el buffer ciclico
uint8_t keyboard_actual=0;
uint8_t keyboard_last=0;
uint8_t size = 0;


uint8_t keyboard_is_full(){
	return (size >= KB_SIZE - 2);
}

uint8_t keyboard_is_empty() {
	return (keyboard_actual == keyboard_last);
}

void keyboard_clear() {
	keyboard_actual = 0;
	keyboard_last = 0;
	size = 0;
}
uint8_t keyboard_size(){
	return size;
}
void flush_buffer(){
	keyboard_actual=keyboard_last;
}
uint8_t keyboard_set_key(uint8_t c) {

	if( !keyboard_is_full() ) {
		if(c == '\b'){
			if(size > 0){
				keyboard_last--;
				size--;
			}
		}else if(c == 0){
			//NOP
		}else{
			keyboard_buffer[keyboard_last++] = c;
			size++;
		}
		if( keyboard_last == KB_SIZE )
			keyboard_last = 0;

		if( c == '\n'){
			load_user_buffer();
		}
		return 1;

	}else if(c == '\b'){
		keyboard_last--;
		size--;
		return 1;
	}else if( c == '\n'){
		keyboard_buffer[keyboard_last++] = c;
		size++;
		if( keyboard_last == KB_SIZE )
			keyboard_last = 0;
		load_user_buffer();
		return 1;
	}

	return 0;
}

uint8_t keyboard_get_key(){
	uint8_t c;

	while(keyboard_is_empty()) {}

	c = keyboard_buffer[keyboard_actual++];
	if( keyboard_actual == KB_SIZE )
		keyboard_actual = 0;

	if(size > 0)
		size--;
	return c;
}

void load_user_buffer(){
	while(keyboard_actual != keyboard_last){
		clean_set_char(keyboard_get_key());
	}
	check_clear_buffer();
}
int get_dirty(char* b,int s){

	int i = 0;
	if(keyboard_actual < keyboard_last){
				while(i + keyboard_actual < keyboard_last && i < s){
					b[i] = keyboard_buffer[keyboard_actual + i];
					i++;
				}
				return i;
	}else if(keyboard_last == keyboard_actual){
		return 0;
	}else{
		int dist = KB_SIZE - keyboard_actual + keyboard_last;
		while(i < dist && i < s){
			b[i] = keyboard_buffer[(keyboard_actual + i) % KB_SIZE];
			i++;
		}
		return i;
	}

}
