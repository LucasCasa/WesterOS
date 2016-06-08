#ifndef HANDLERS_H_
#define HANDLERS_H_

#include "video_manager.h"
#include "screensaver.h"
#include "keyboard.h"
#include "keyboard_buffer.h"

void timer_handler();
void keyboard_handler(uint8_t scancode);
void setPiano();

extern void shell_show_screensaver();
extern void shell_print_standby();
extern int draw_mode;
#endif
