#ifndef HANDLERS_H_
#define HANDLERS_H_

#include "video_manager.h"
#include "screensaver.h"
#include "keyboard.h"
#include "keyboard_buffer.h"

#define MINSCRSAVERTIME 3600 //1 hour

#define PIT_FREQ 	100 // 100 veces x segundo = 10ms

void timer_handler();
void keyboard_handler(uint8_t scancode);
void setPiano();
int validateScreenTime(int time);

extern void shell_show_screensaver();
extern void shell_print_standby();
extern int draw_mode;
#endif
