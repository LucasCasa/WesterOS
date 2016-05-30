#ifndef SYS_LIB_H_
#define SYS_LIB_H_

#include "video_manager.h"

void * memset(void * destination, int32_t c, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int isNumber(char a);
void print_message(char * message, uint8_t mod);
void print_number(uint32_t n);

#endif