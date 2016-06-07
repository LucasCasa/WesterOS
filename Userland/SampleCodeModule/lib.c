#include <stdint.h>
#include <stdarg.h>
#include "lib.h"
#include "call80.h"
int isNumber(char a);

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	uint8_t * dst = (uint8_t*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void print_message(char* message, uint8_t mod){
	int i = 0;
	while(message[i] != 0){
		_put_char(message[i],mod);
		i++;
	}
}

void print_number(uint32_t n){
	if(n < 10){
		_put_char(n + '0',0xFF);
	}else{
		print_number(n / 10);
		_put_char((n % 10) + '0',0xFF);
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
int atoi(char * string){
	int num=0;
	while(*string != 0 && num>=0){		
		num = num*10;
		if(isNumber(*string))
			num=num+(*string-'0');
		else 
			num=-1;
		string++;
	}
	return num;
}
int strlen(const char * str){
	int size =0;
	for(int i=0;str[i]!=0;i++){
		size++;
	}
	return size;
}

int isNumber(char a){
	if(a <= '9' && a >= '0')
		return 1;
	else
		return 0;
}

int pow(int x, unsigned int y){
	int resp=1;
	for(int i=1;i<y;i++){
		resp*=x;
	}
	return resp;
}
void putChar(uint8_t c,uint8_t mod){
	_put_char(c,mod);
}

void printf(const char * string, uint8_t mod, ...){
	char c;
	va_list listpointer;
	va_start(listpointer, mod);
	for(int i=0; string[i] != '\0'; i++){
		c = string[i];
		if(c == '%'){
			switch(string[i+1]){
				case 'd':	print_number(va_arg(listpointer,int));
							i++;
							break;
				case 's':	print_message(va_arg(listpointer,char *), mod);
							i++;
							break;
				default: 	_put_char(string[i], mod);
							break;
			}
		}
		else{
			_put_char(string[i], mod); // 15 para fondo negro letras blancas
		}
	}
	va_end(listpointer);
}
char get_char(){
	char c;
	do{
		c = _call_int80(INT_GET_CHAR);
	}while(c == 0);
	return c;
}

void flush_buffer(){
	char c = 0;
	while ((c=get_char()) != '\n'){};
	return;
}

void * malloc(uint64_t size){
	return (void *)_call_int80(INT_MALLOC,size);
}

void free(void  * address){
	_call_int80(INT_FREE,(uint64_t)address);
	return;
}