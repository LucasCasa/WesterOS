#include <stdint.h>
#include "lib.h"
#include "call80.h"

void* set_screensaver_time(void*s){
	char value[10];
	uint8_t c = 1;
	int res = 0;
	uint8_t i = 0;
	int flag = 0;
	do{
		do{
			i = 0;
			c = 1;
			print_message("Enter value (in seconds): \n",0xFF);
			while((c = get_char()) != '\n' && i<10){
				value[i] = c;
				i++;
			}
			value[i] = 0;
			res = atoi(value);
		}while(res <= 0);
		flag = _call_int80(INT_SCR_TIME,res);
		if(i==10)
			flush_buffer();
	}while(!flag);

	return 0;
}
