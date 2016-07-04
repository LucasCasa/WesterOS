#ifndef LIB_H
#define LIB_H

#include <stdint.h>

#define INT_WRITE 1
#define INT_GET_CHAR 2
#define INT_GET_STR 3
#define INT_ERASE_SCR 4
#define INT_RTC_READ 5
#define INT_RTC_WRITE 6
#define INT_COLORS 7
#define INT_SCR_TIME 8
#define INT_BEEP 9
#define INT_PIANO 10
#define INT_SONGS 11
#define INT_MALLOC 12
#define INT_FREE 13

/* --- IPC CALLS --- */

#define INT_MKFIFO 20
#define INT_OPENFIFO 21
#define INT_CLOSEFIFO 22
#define INT_WRITEFIFO 23
#define INT_READFIFO 24
#define INT_READFIFOBLOQ 25
#define INT_SHOWIPCS 26

/* --- GRAPHIC SYS CALLS --- */
#define INT_SCREENSAVER   40
#define INT_COLORS_GRAPHIC 41
#define INT_CLEAR 42
#define INT_DRAW_CIRCLE 43
#define INT_DRAW_IMAGE 44
#define INT_DRAW_TEXT 45
#define INT_ENTER_DRAW_MODE 46
#define INT_EXIT_DRAW_MODE 47
#define INT_DRAW_ERASABLE_CIRCLE 48
#define INT_UNDRAW_ERASABLE_CIRCLE 49

#define INT_GCFB  50
#define INT_SET_EVENT_KEYUP 51
#define INT_UNSET_EVENT_KEYUP 52
#define INT_SET_EVENT_KEYDOWN 53
#define INT_UNSET_EVENT_KEYDOWN 54

#define INT_NEW_PROCESS              69
#define INT_END_PROCESS              70
#define INT_GET_ALL_PROCESS          71


#define INT_SLEEP 80




void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

int isNumber(char a);

void print_message(char* message, uint8_t mod);
void print_number(uint32_t n);

int strcmp(char *s1, char *s2);
int atoi(char * string);
int strlen(const char * str);
int isNumber(char a);
int pow(int x, unsigned int y);
void putChar(uint8_t c,uint8_t mod);
void printf(const char * string, uint8_t mod, ...);
char get_char();
void list_ipcs();

void flush_buffer();

int rand(void);

typedef struct Point{
   int x;
   int y;
} Point;

typedef struct Color{
   int r;
   int g;
   int b;
}Color;

#endif
