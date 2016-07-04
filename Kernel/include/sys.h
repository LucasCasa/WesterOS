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
#include "allocator.h"
#include "ipc.h"


#define WRITE        1
#define GET_CHAR     2
#define GET_STR      3
#define ERASE_SCR    4
#define RTC_READ     5
#define RTC_WRITE    6
#define COLORS       7
#define SCR_TIME     8
#define BEEP         9
#define PIANO        10
#define SONGS        11
#define MALLOC       12
#define FREE         13
#define GET_DIRTY_STR 14

/* --- IPC CALLS --- */

#define MKFIFO       20
#define OPENFIFO     21
#define CLOSEFIFO    22
#define WRITEFIFO    23
#define READFIFO     24
#define READFIFOBLOQ 25
#define SHOWIPCS	 26

/* --- GRAPHIC SYS CALLS --- */
#define SET_SCREENSAVER          40
#define COLORS_GRAPHIC           41
#define CLEAR                    42
#define DRAW_CIRCLE              43
#define DRAW_IMAGE               44
#define DRAW_TEXT                45
#define ENTER_DRAW_MODE          46
#define EXIT_DRAW_MODE           47
#define DRAW_ERASABLE_CIRCLE     48
#define UNDRAW_ERASABLE_CIRCLE   49

#define GET_CHAR_FROM_BUFFER     50
#define SET_EVENT_KEYUP          51
#define UNSET_EVENT_KEYUP        52
#define SET_EVENT_KEYDOWN        53
#define UNSET_EVENT_KEYDOWN      54

/* --- MULTITASKING SYS CALLS --- */

#define NEW_PROCESS              69
#define END_PROCESS              70
#define GET_ALL_PROCESS          71

#define SLEEP                    80



char read(char* buff, uint8_t size);
char read_char();
uint64_t sys_manager(int order, uint64_t arg1, uint64_t arg2,uint64_t arg3);
extern uint32_t screensaver_time;
char get_char_from_buffer();

#endif
