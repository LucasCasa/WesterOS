#ifndef KEYBOARD_BUFFER_H
#define KEYBOARD_BUFFER_H
#define KB_SIZE 0xFF

#include <stdint.h>
#include "clean_buffer.h"

uint8_t keyboard_is_full();
uint8_t keyboard_is_empty();
void keyboard_clear();
uint8_t keyboard_size();
uint8_t keyboard_set_key(uint8_t c);
uint8_t keyboard_get_key();
void load_user_buffer();

#endif
