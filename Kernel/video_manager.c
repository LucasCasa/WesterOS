#include "video_manager.h"
void write_serial(char a);
static uint8_t * video = (uint8_t*) 0xB8000;
static uint8_t * currentVideo = (uint8_t*) 0xB8000;
static uint8_t * saved_current_video;
static uint8_t * saved_command_line;
static uint8_t * command_line = (uint8_t*) 0xB8000;
extern void _change_to_graphics();
Image image;

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
	currentVideo = video;
	draw_new_line();
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
void graphic_screensaver(){
	uint8_t* st = (*(uint32_t*)0x5080);

	int offsetx = 400;
	int offsety = 400;
	int modx = 1;
	int mody = 1;
	int offsetx2 = 1;
	int offsety2 = 1;
	int modx2 = 1;
	int mody2 = 1;
	while(1){

	int totaloffset = offsety*1024*3 + offsetx*3;
	int totaloffset2 = offsety2*1024*3 + offsetx2*3;

	for(int i = 0; i<image.height;i++){
		for(int j = 0;j<image.width*3;j++){
			st[totaloffset + j+2 + 1024*i*3] = image.pixel_data[j + i*image.width*3];
			j++;
			st[totaloffset + j + 1024*i*3] = image.pixel_data[j + i*image.width*3];
			j++;
			st[totaloffset + j-2 + 1024*i*3] = image.pixel_data[j + i*image.width*3];
		}
	}
	if(offsety + image.height >= 768){
		mody = -1;
	}else if(offsety <= 0){
		mody = 1;
	}
	if(offsetx + image.width >= 1024){
		modx = -1;
	}else if(offsetx <= 0){
		modx = 1;
	}
	offsetx+= modx;
	offsety+= mody;
/*
		for(int i = 0; i<gimp_image2.height;i++){
			for(int j = 0;j<gimp_image2.width*3;j++){
				st[totaloffset2 + j+2 + 1024*i*3] = gimp_image2.pixel_data[j + i*gimp_image2.width*3];
				j++;
				st[totaloffset2 + j + 1024*i*3] = gimp_image2.pixel_data[j + i*gimp_image2.width*3];
				j++;
				st[totaloffset2 + j-2 + 1024*i*3] = gimp_image2.pixel_data[j + i*gimp_image2.width*3];
			}
		}
		if(offsety2 + gimp_image2.height >= 768){
			mody2 = -1;
		}else if(offsety2 <= 0){
			mody2 = 1;
		}
		if(offsetx2 + gimp_image2.width >= 1024){
			modx2 = -1;
		}else if(offsetx2 <= 0){
			modx2 = 1;
		}
		offsetx2+= modx2;
		offsety2+= mody2;*/
	}
}
void erase_screen(){
	for(int j = 0; j<25*160;j++){
		video[j++] = 0;
		video[j] = (num_modifier & 0xF0) + (num_modifier >> 4);
	}
}
int is_transmit_empty() {
   return inb(0x3F8 + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(0x3F8,a);
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
