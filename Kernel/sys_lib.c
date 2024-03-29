#include <stdint.h>
#include "sys_lib.h"
static int next = 1;
void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	uint8_t * dst = (uint8_t*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int isNumber(char a){
	if(a <= '9' && a >= '0')
		return 1;
	else
		return 0;
}
char toUpper(char c){
	if(c >= 'a' && c<= 'z'){
		return c + 'A' - 'a';
	}
	return c;
}
void print_message(char* message, uint8_t mod){
	int i = 0;
	while(message[i] != 0){
		sys_write(message[i],mod);
		i++;
	}
}

void print_number(uint32_t n){
	if(n<10){
		sys_write(n + '0',0xFF);
	}else{
		print_number(n/10);
		sys_write((n%10)+'0',0xFF);
	}
}

int strcmp(char *s1, char *s2) {
  int ret = 0;
  while (!(ret = *(uint8_t *) s1 - *(uint8_t *) s2) && *s2) ++s1, ++s2;
  if (ret < 0)
    ret = -1;
  else if (ret > 0)
    ret = 1 ;
  return ret;
}

int rand(void){ // RAND_MAX assumed to be 32767
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}
void srand(unsigned int seed) {
    next = seed;
}
