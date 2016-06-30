#include "lib.h"
#include "call80.h"
#include "colors.h"

Image* imgs = 0x710000;
void modify_colors(){
	print_message("Select Colors:\n",0xFF);
	print_message("1- GREYJOY\n",0xFF);
	print_message("2- TYRELL \n",0xFF); //TYRELL
	print_message("3- LANNISTER \n",0xFF); // LANNISTER
	print_message("4- STARK \n",0xFF); //STARK
	print_message("5- TARGARYEN\n",0xFF); //TARGARYEN
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
			fr.r = 255;
			fr.g = 255;
			fr.b = 0;

			bk.r = 0;
			bk.g = 0;
			bk.b = 0;

			num.r = 255;
			num.g = 0;
			num.b = 0;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			_call_int80(INT_SCREENSAVER,&imgs[0]);
			break;
		case 2:
			_call_int80(INT_COLORS,0x1F,0x12);
			fr.r = 255;
			fr.g = 255;
			fr.b = 30;

			bk.r = 0;
			bk.g = 200;
			bk.b = 0;

			num.r = 30;
			num.g = 30;
			num.b = 30;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			_call_int80(INT_SCREENSAVER,&imgs[1]);
			break;
		case 3:
			_call_int80(INT_COLORS,0x0E,0x0B);
			fr.r = 255;
			fr.g = 255;
			fr.b = 0;

			bk.r = 255;
			bk.g = 0;
			bk.b = 0;

			num.r = 255;
			num.g = 255;
			num.b = 100;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			_call_int80(INT_SCREENSAVER,&imgs[2]);
			break;
		case 4:
			_call_int80(INT_COLORS,0x70,0x71);
			fr.r = 255;
			fr.g = 255;
			fr.b = 255;

			bk.r = 128;
			bk.g = 128;
			bk.b = 128;

			num.r = 0;
			num.g = 0;
			num.b = 0;
			_call_int80(INT_COLORS_GRAPHIC,&fr,&bk,&num);
			_call_int80(INT_SCREENSAVER,&imgs[3]);
			break;
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
			_call_int80(INT_SCREENSAVER,&imgs[4]);
			break;
	}
}
