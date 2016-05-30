#include "lib.h"
#include "call80.h"

void modify_colors(){
	print_message("Select Colors:\n",0xFF);
	print_message("1- Black Background, Green Letters,  Red Numbers  (default)\n",0xFF);
	print_message("2- Blue Background,  White Letters,  Green Numbers  \n",0xFF);
	print_message("3- Black Background, Yellow Letters, Sky Blue Numbers  \n",0xFF);
	print_message("4- Grey Background,  Black Letters,  Blue Numbers  \n",0xFF);
	print_message("5- Black Background, Red Letterss,   Whilte Numbers  \n",0xFF);
	char c = 0,i = 0,res = 0;
	do{
	c = 0;
	i = 0;
	res = 0;
	print_message("Enter value:\n",0xFF);
	while( (c = get_char())  != '\n'){
		i++;
		//print_message(c);
		if(isNumber(c)){
			res = (c - '0') ;
		}else{
			res = 0;
		}
	}
	}while(i > 1 || res == 0 || res > 5);

	switch(res){
		case 1:
			_call_int80(INT_COLORS,0x02,0x04);
			break;
		case 2:
			_call_int80(INT_COLORS,0x1F,0x12);
			break;
		case 3:
			_call_int80(INT_COLORS,0x0E,0x0B);
			break;
		case 4:
			_call_int80(INT_COLORS,0x70,0x71);
			break;
		case 5:
			_call_int80(INT_COLORS,0x04,0x0F);
			break;
	}
}
