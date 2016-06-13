#ifndef _GAME_H
#define _GAME_H

	#include "lib.h"

	#define RADIUS      4
	#define MAX_PLAYERS 6 // DOBLE TECLADO SE PUEDEN HASTA 6??
	#define HOLE_SIZE   10
	#define MAX_DRAW    90
	#define COLOR_POS   50
	#define NUMBER_POS  180
	#define RIGHT_POS   310
	#define LEFT_POS    500

	typedef struct Position{
		int x;
		int y;
	}Position;

	typedef struct PowerUp{
		int radius;
		Position pos;
		
	}PowerUp;

	extern double _sin(int angle);
	extern double _cos(int angle);

	void get_key_up(uint8_t);
	void get_key_down(uint8_t);
	char draw_into_board(uint32_t pn,Point p);
	void exit_game();
	
#endif