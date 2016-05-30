#ifndef VIDEO_MANAGER_H
#define VIDEO_MANAGER_H

#include "stdint.h"
#include "sys_lib.h"

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

#endif
