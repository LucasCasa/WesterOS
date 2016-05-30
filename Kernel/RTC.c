#include <stdint.h>
#include "rtc.h"

extern void _set_value(char r,uint8_t value);
extern uint8_t _get_value(uint8_t);

uint8_t BCDtoDecimal(uint8_t num){
	int aux = 0;
	aux = (num >> 4) * 10;
	aux += (num & 15);
	return aux;
}
uint8_t DecimaltoBCD(uint8_t n){
	int result = 0;
	while(n>10){
		result++;
		n -= 10;
	}
	result = (result << 4);
	result += n;
	return result;
}
unsigned char RTCparameters(char arg){
	switch(arg){
		case 0: return BCDtoDecimal(_get_value(SEC));
		case 1: return BCDtoDecimal(_get_value(MIN));
		case 2: return BCDtoDecimal(_get_value(HORA));
		case 3: return BCDtoDecimal(_get_value(DIA));
		case 4: return BCDtoDecimal(_get_value(MES));
		case 5: return BCDtoDecimal(_get_value(ANIO));
	}
	return 0;
}
void set_date(char arg, uint32_t value){
	switch(arg){
		case 0: _set_value(ANIO, DecimaltoBCD((value % 100)));
		break;
		case 1: _set_value(MES, DecimaltoBCD(value));
		break;
		case 2: _set_value(DIA, DecimaltoBCD(value));
		break;
		case 3: _set_value(HORA, DecimaltoBCD(value));
		break;
		case 4: _set_value(MIN, DecimaltoBCD(value));
		break;
		case 5: _set_value(SEC, DecimaltoBCD(value));
		break;
	}
}
