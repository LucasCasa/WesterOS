#include "video_manager.h"

static uint8_t * const video = (uint8_t*) 0xA0000;
static uint8_t * currentVideo = (uint8_t*) 0xA0000;
static uint8_t * saved_current_video;
static uint8_t * saved_command_line;
static uint8_t * command_line = (uint8_t*) 0xA0000;

uint8_t str_modifier = 0x02;
uint8_t num_modifier = 0x04;

uint8_t saved_shell[160*25];
uint8_t saved_modifier;


void set_default_modifiers(char s, char n){
	str_modifier = s;
	num_modifier = n;
	set_new_modifier();
}
void set_new_modifier(){
	for(int i = 0; i<160*25;i++){
		if(isNumber((char) video[i]) || i % 160 < 4){
			video[++i] = num_modifier;
		}else{
			video[++i] = str_modifier;
		}
	}
}
char get_modifier(){
	return *(currentVideo +1);
}
void modify(char mod){
	*(currentVideo + 1) = mod;
}
char sys_get_screen_char(){
	return *(currentVideo - 2);
}
void sys_delete_char(){
	if(currentVideo - command_line > 4){
		*(currentVideo + 1) = str_modifier;
		currentVideo -=2;
		*(currentVideo) = 0;
		*(currentVideo +1) = str_modifier;
	}
}
void draw_new_line(){
	*(currentVideo++) = '>';
	*(currentVideo++) = num_modifier;
	*(currentVideo++) = ':';
	*(currentVideo++) = num_modifier;
	set_command_line();
}
void reset_current_video(){
	/*currentVideo = video;
	draw_new_line();*/
	set_command_line();
}
void save_screen(){
	for(int i = 0; i<160*25;i++){
		saved_shell[i] = video[i];
	}
	saved_current_video = currentVideo;
	saved_command_line = command_line;
	currentVideo = video;
}
void restore_screen(){
	for(int i = 0; i<160*25;i++){
		video[i] = saved_shell[i];
	}
	currentVideo = saved_current_video;
	command_line = saved_command_line;
}
void new_line(){
	*currentVideo = 0;
	*(currentVideo+1) = saved_modifier;
	uint64_t aux;
	aux = (uint64_t)currentVideo - 0xB8000;
	currentVideo = (uint8_t*)(0xB8000 + (aux + 160) - (aux % 160));
	draw_new_line();
}
void sys_write(char c,uint8_t mod){
	char aux = 0;
	switch(c){
		case '\n':
			new_line();
			aux = 1;
			break;
		case '\b':
			sys_delete_char();
		case 0:
			break;
		default:
			*(currentVideo++) = c;
			if(mod == 0xFF){
				if(isNumber(c)){
					*(currentVideo++) = num_modifier;
				}else{
					*(currentVideo++) = str_modifier;
				}
			}else{
				*(currentVideo++) = mod;
			}
			break;
	}
	check_end_of_screen(aux);
}

char check_end_of_screen(char type){
	if(currentVideo >= (uint8_t *)(0xB8000 + 160*25)){
		scroll();
		currentVideo = (uint8_t *)(0xB8000 + 160*24);
		command_line -= 160;
		if(type == 1){
			draw_new_line();
		}
		return 1;
	}
	return 0;
}
void scroll(){
	int j = 0;
	for(int i = 160;i<160*25;i++,j++){
		video[j] = video[i];
		video[i] = 0;
	}
	while( j < 160*25){
		video[++j] = str_modifier;
		j++;
	}
}

void erase_screen(){
	/*
	for(int j = 0; j<25*160;j++){
		video[j++] = 0;
		video[j] = (num_modifier & 0xF0) + (num_modifier >> 4);
	}*/
	for(int j = 0; j<9*(1024*128);j++){
		*(currentVideo++) = 'A';
		currentVideo++;
	}
}
void print_standby(){
	if(get_modifier() == 0x22){
		modify(saved_modifier);
	}else{
		saved_modifier = get_modifier();
		modify(0x22);
	}

}
void set_command_line(){
	command_line = currentVideo - ((uint64_t)(currentVideo - 0xB8000) % 160);
}
