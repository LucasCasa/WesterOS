#ifndef SYS_H_
#define SYS_H_

#include "stdint.h"
#include "sys_lib.h"
#include "clean_buffer.h"
#include "rtc.h"
#include "screensaver.h"
#include "sounds.h"
#include "handlers.h"
#include "typedef.h"


#define WRITE 1
#define GET_CHAR 2
#define GET_STR 3
#define ERASE_SCR 4
#define RTC_READ 5
#define RTC_WRITE 6
#define COLORS 7
#define SCR_TIME 8
#define BEEP 9
#define PIANO 10
#define SONGS 11

/* --- GRAPHIC SYS CALLS --- */

#define CLEAR 42
#define DRAW_CIRCLE 43
#define DRAW_IMAGE 44
#define DRAW_TEXT 45
#define ENTER_DRAW_MODE 46
#define EXIT_DRAW_MODE 47

#define GET_CHAR_FROM_BUFFER 50

char read(char* buff, uint8_t size);
char read_char();
char sys_manager(int order, uint64_t arg1, uint64_t arg2,uint64_t arg3);
extern uint32_t screensaver_time;
char get_char_from_buffer();

#endif
