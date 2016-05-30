#ifndef KEAYBOARD_H_
#define	KEYBOARD_H_

uint8_t check_special_key(uint32_t c);
int is_alpha(unsigned char scancode);

uint8_t scancode_to_char(uint32_t c);
uint32_t getFrec(uint32_t scancode);


#endif