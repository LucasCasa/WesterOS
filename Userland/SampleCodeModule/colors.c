#include "lib.h"
#include "call80.h"

void modify_colors(){
	print_message("Select Colors:\n",0xFF);
	print_message("1- Black Background, Green Letters,  Red Numbers  (default)\n",0xFF);
	print_message("2- Blue Background,  White Letters,  Green Numbers  \n",0xFF);
	print_message("3- Black Background, Yellow Letters, Sky Blue Numbers  \n",0xFF);
	print_message("4- Grey Background,  Black Letters,  Blue Numbers  \n",0xFF);
	print_message("5- Black Background, Red Letters,   White Numbers  \n",0xFF);
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
	Color fr;
	Color bk;
	Color num;
	switch(res){
		case 1:
			_call_int80(INT_COLORS,0x02,0x04);
			fr.r = 0;
			fr.g = 255;
			fr.b = 0;

			bk.r = 0;
			bk.g = 0;
			bk.b = 0;

			num.r = 255;
			num.g = 0;
			num.b = 0;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			break;
		case 2:
			_call_int80(INT_COLORS,0x1F,0x12);
			fr.r = 255;
			fr.g = 255;
			fr.b = 255;

			bk.r = 0;
			bk.g = 0;
			bk.b = 255;

			num.r = 0;
			num.g = 255;
			num.b = 0;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			break;
		case 3:
			_call_int80(INT_COLORS,0x0E,0x0B);
			fr.r = 255;
			fr.g = 255;
			fr.b = 0;

			bk.r = 0;
			bk.g = 0;
			bk.b = 0;

			num.r = 0;
			num.g = 255;
			num.b = 255;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			break;
		case 4:
			_call_int80(INT_COLORS,0x70,0x71);
			fr.r = 0;
			fr.g = 0;
			fr.b = 0;

			bk.r = 192;
			bk.g = 192;
			bk.b = 192;

			num.r = 0;
			num.g = 0;
			num.b = 255;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
		case 5:
			_call_int80(INT_COLORS,0x04,0x0F);
			fr.r = 255;
			fr.g = 0;
			fr.b = 0;

			bk.r = 0;
			bk.g = 0;
			bk.b = 0;

			num.r = 255;
			num.g = 255;
			num.b = 255;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			break;
	}
}
