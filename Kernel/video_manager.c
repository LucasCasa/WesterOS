#include "video_manager.h"
#include "letter.h"
static FontImage* font = (FontImage*)0x800000;
extern draw_mode;
void write_serial(char a);
static uint8_t * video = (uint8_t*) 0xB8000;
static uint8_t * currentVideo = (uint8_t*) 0xB8000;
static uint8_t * saved_current_video;
static uint8_t * saved_command_line;
int command_line_graphic = 0;
static uint8_t * command_line = (uint8_t*) 0xB8000;
uint8_t * graphic_video = 0;
uint8_t * current_graphic_video = 0;
uint8_t str_modifier = 0x02;
uint8_t num_modifier = 0x04;
uint8_t saved_shell[160*25];
uint8_t saved_modifier;
uint8_t* saved_shell_graphic = (uint8_t* ) 0xA000000;
uint8_t* saved_current_video_graphic = 0;
uint8_t* saved_command_line_graphic = 0;
Color font_color = {0,255,0};
Color background_color = {0,0,0};
Color number_color = {255,0,0};
char stand_by_active = 0;
int gm = 1;

void set_new_modifier_graphic(Color* nf, Color* nb,Color* nnum);
void put_graphics(char c);
void set_command_line_graphic();
void put_char(char c, uint8_t mod);
void print_standby_graphic();


void set_default_modifiers(char s, char n){
	str_modifier = s;
	num_modifier = n;
	set_new_modifier();
}
void set_default_modifiers_graphic(Color* front, Color* back,Color* num){
	set_new_modifier_graphic(front,back,num);
	font_color.b = front->b;
	font_color.r = front->r;
	font_color.g = front->g;

	background_color.r = back->r;
	background_color.g = back->g;
	background_color.b = back->b;

	number_color.r = num->r;
	number_color.g = num->g;
	number_color.b = num->b;
}
void set_new_modifier_graphic(Color* nf, Color* nb,Color* nnum){
	Color newc;
	for(int i = 0; i<1024*768*3;i++){
		if(graphic_video[i] == font_color.b && graphic_video[i+1] == font_color.g && graphic_video[i+2] == font_color.r){
			newc.r = nf->r;
			newc.g = nf->g;
			newc.b = nf->b;
		}else if(graphic_video[i] == background_color.b && graphic_video[i+1] == background_color.g && graphic_video[i+2] == background_color.r){
			newc.r = nb->r;
			newc.g = nb->g;
			newc.b = nb->b;
		}else if(graphic_video[i] == number_color.b && graphic_video[i+1] == number_color.g && graphic_video[i+2] == number_color.r){
			newc.r = nnum->r;
			newc.g = nnum->g;
			newc.b = nnum->b;
		}else{
			newc.r = graphic_video[i+2];
			newc.g = graphic_video[i+1];
			newc.b = graphic_video[i];
		}
		graphic_video[i++] = newc.b;
		graphic_video[i++] = newc.g;
		graphic_video[i] = newc.r;

	}

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
void sys_delete_char_graphic(){
	draw_char_graphic(0);
	if((command_line_graphic != get_yoffset()) || (get_char_xoffset() > 2)){
		if(((current_graphic_video - graphic_video) % (1024*3)) == 0){
			current_graphic_video -= 1024*3*(LETTER_HEIGHT);
			current_graphic_video += (LETTER_WIDTH*3) * (((1024) / (LETTER_WIDTH) )-1);
		}else{
			current_graphic_video -= LETTER_WIDTH*3;
		}
		draw_char_graphic(0);
	}
}
uint32_t get_char_xoffset(){
	return get_xoffset() / (LETTER_WIDTH*3); // ACA USO LETTER WIDTH; SI EN ALGUN MOMENTO LO CAMBIO MIRAR
}
uint32_t get_char_yoffset(){
	return get_yoffset() / (LETTER_HEIGHT*3);
}
uint32_t get_xoffset(){
	int o = (current_graphic_video - graphic_video) % (1024*3);
	return o;
}
uint32_t get_yoffset(){ // retorna el numero de linea sobre el eje y
	int o = (current_graphic_video - graphic_video) / (1024*3);
	return o;
}
void draw_new_line(){
	*(currentVideo++) = '>';
	*(currentVideo++) = num_modifier;
	*(currentVideo++) = ':';
	*(currentVideo++) = num_modifier;
	set_command_line();
}
void draw_new_line_graphic(){
	put_graphics('>');
	put_graphics(':');
	set_command_line_graphic();
}
void reset_current_video(){
	currentVideo = video;
	if(!draw_mode)
		draw_new_line();
	set_command_line();
}
void reset_current_video_graphic(){
	current_graphic_video = graphic_video;
	if(!draw_mode){
		draw_new_line_graphic();
		set_command_line_graphic();
	}
}
void save_screen(){
	for(int i = 0; i<160*25;i++){
		saved_shell[i] = video[i];
	}
	saved_current_video = currentVideo;
	saved_command_line = command_line;
	currentVideo = video;
}
void save_screen_graphic(){
	for (int i = 0; i<1024*768*3;i++){
			saved_shell_graphic[i] = graphic_video[i];
	}
	saved_current_video_graphic = current_graphic_video;
	saved_command_line_graphic = command_line_graphic;
	current_graphic_video = graphic_video;
}
void restore_screen(){
	for(int i = 0; i<160*25;i++){
		video[i] = saved_shell[i];
	}
	currentVideo = saved_current_video;
	command_line = saved_command_line;
}
void restore_screen_graphic(){
	for(int i = 0; i<1024*768*3;i++){
		graphic_video[i] = saved_shell_graphic[i];
	}
	current_graphic_video = saved_current_video_graphic;
	command_line_graphic = saved_command_line_graphic;
}
void new_line(){
	*currentVideo = 0;
	*(currentVideo+1) = saved_modifier;
	uint64_t aux;
	aux = (uint64_t)currentVideo - 0xB8000;
	currentVideo = (uint8_t*)(0xB8000 + (aux + 160) - (aux % 160));
	draw_new_line();

}
void new_line_graphic(){
	if(graphic_video == 0){
		graphic_video = (*(uint32_t*)0x5080);
		current_graphic_video = (*(uint32_t*)0x5080);
	}
	if(stand_by_active){
		draw_char_graphic(0);
	}
	int currentline = get_yoffset();
	if(currentline + LETTER_HEIGHT > 768){
			scroll_graphic(1);
	}else{
			current_graphic_video = graphic_video + 1024*3*(currentline + LETTER_HEIGHT);
	}
	draw_new_line_graphic();
}
void draw_char_graphic(char c){
	/*int totaloffset = 0;
	int fontoffset = 0;
	uint8_t* start;
	if(c < 32){
	 start = font->pixel_data;
	}else{
	 start = font->pixel_data + 21*(c - 32)*3;
	}
	for(int i = 0; i<LETTER_HEIGHT;i++){
		for(int j = 0; j<LETTER_WIDTH*3;j++){
			current_graphic_video[totaloffset + j] =start[fontoffset + j + 2 ] ;
			j++;
			current_graphic_video[totaloffset + j] = start[j + fontoffset];
			j++;
			current_graphic_video[totaloffset + j] = start[j-2 + fontoffset];
		}
		totaloffset += 1024*3;
		fontoffset += font->width*3;
	}*/
	draw_char_graphic_basic(c,0,0);
}
void draw_char_graphic_basic(char c,Color front,Color back){
	int totaloffset = 0;
	int l = 0;
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	for(int i = 0; i<LETTER_HEIGHT;i++){
		for(int j = 0; j<LETTER_WIDTH*3;j++){
			if(lettersf[c].pixel_data[l] && !isNumber(c)){
				r = font_color.r;
				g = font_color.g;
				b = font_color.b;
			}else if(lettersf[c].pixel_data[l]){
				r = number_color.r;
				g = number_color.g;
				b = number_color.b;
			}else{
				r = background_color.r;
				g = background_color.g;
				b = background_color.b;
			}
			current_graphic_video[totaloffset + j] = b;
			j++;
			current_graphic_video[totaloffset + j] = g;
			j++;
			current_graphic_video[totaloffset + j] = r;
			l++;
		}

		totaloffset += 1024*3;
	}
}
void sys_write(char c,uint8_t mod){
	char aux = 0;
	switch(c){
		case '\n':
			new_line();
			new_line_graphic();
			aux = 1;
			break;
		case '\b':
			sys_delete_char();
			sys_delete_char_graphic();
		case 0:
			break;
		default:
			put_char(c,mod);
			put_graphics(c);
		break;
	}
	check_end_of_screen(aux);
}
void put_char(char c,uint8_t mod){
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
}
void put_graphics(char c){
	//int totaloffset = 0;
	//uint32_t fontoffset = 0;
	if(graphic_video == 0){
	graphic_video = (*(uint32_t*)0x5080);
	current_graphic_video = (*(uint32_t*)0x5080);
	}
	if(end_of_line()){
		int off = get_yoffset();
		int off2 = 1024*3*(LETTER_HEIGHT + off);
		current_graphic_video = graphic_video + off2;
	}
	check_end_of_screen_graphic(0);
	draw_char_graphic(c);
	current_graphic_video = current_graphic_video + LETTER_WIDTH*3; //*3 con la otra font
}
uint8_t end_of_line(){
	return (((current_graphic_video - graphic_video) % (1024*3)) > (((current_graphic_video - graphic_video) + LETTER_WIDTH*3) % (1024*3)));
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
char check_end_of_screen_graphic(char type){
	if(get_yoffset() + LETTER_HEIGHT > 768){
		scroll_graphic();
		current_graphic_video = graphic_video + (get_yoffset()-LETTER_HEIGHT)*1024*3;
		if(type == 1){
			draw_new_line_graphic();
		}
		return 1;
	}
	return 0;
}
void scroll_graphic(){
	int j = 0;
	//int jump = 1024*3*LETTER_HEIGHT;
	for(int i = 1024*3*LETTER_HEIGHT;i<1024*768*3;i++,j++){
		graphic_video[j] = graphic_video[i];
		graphic_video[i] = 0;
	}
	while( j < 1024*768*3){
		graphic_video[++j] = 0;
	}
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
 	print_standby_graphic();
}
void print_standby_graphic(){
	Color c;
	if(!stand_by_active){
		c.r = font_color.r;
		c.g = font_color.g;
		c.b = font_color.b;
		stand_by_active = 1;
	}else{
		c.r = background_color.r;
		c.g = background_color.g;
		c.b = background_color.b;
		stand_by_active = 0;
	}
	int totaloffset = 0;
	for(int i = 0; i<LETTER_HEIGHT;i++){
		for(int j = 0; j<LETTER_WIDTH*3;j++){
			current_graphic_video[totaloffset + j++] = c.b;
			current_graphic_video[totaloffset + j++] = c.g;
			current_graphic_video[totaloffset + j] = c.r;
		}
		totaloffset+=1024*3;
	}
}
void set_command_line(){
	command_line = currentVideo - ((uint64_t)(currentVideo - 0xB8000) % 160);
}
void set_command_line_graphic(){
	command_line_graphic = get_yoffset();
}
