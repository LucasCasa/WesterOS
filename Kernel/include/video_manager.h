#ifndef VIDEO_MANAGER_H
#define VIDEO_MANAGER_H

#include "stdint.h"
#include "typedef.h"
#include "sys_lib.h"
#include "image.h"

void sys_write(char c,uint8_t mod);
char sys_get_screen_char();
void sys_delete_char();
void modify(char mod);
char check_end_of_screen(char type);
void set_default_modifiers(char s, char n);
void set_new_modifier();
void draw_new_line();
void reset_current_video();
void save_screen();
void restore_screen();
void new_line();
void scroll();
void erase_screen();
void print_standby();
void set_command_line();

uint32_t get_char_xoffset();
uint32_t get_char_yoffset();
uint32_t get_xoffset();
uint32_t get_yoffset();
uint8_t end_of_line();
void scroll_graphic();
void draw_char_graphic(char c);
char check_end_of_screen_graphic(char type);

#endif //29880
