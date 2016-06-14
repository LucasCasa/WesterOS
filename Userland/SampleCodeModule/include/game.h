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
	
	#define WIDTH 1024
	#define HEIGHT 768

	#define NUM_POWERUPS 1
	#define MAX_POWERUPS 3
	#define MIN_POWERUP_TIME 50
	#define MAX_POWERUP_TIME 500
	#define POWERUP_RADIUS 15

	/*typedef struct Position{
		int x;
		int y;
	}Position;*/

	typedef struct Vector{
		double x;
		double y;
	}Vector;

	typedef struct Player{
		int alive;
		Point pos;
		Vector acum;
		int radius;
		int angle;
		int mod;
		int time_with_inv;
		int time_no_inv;
		int erasable;
	}Player;

	typedef struct PowerUp{
		int active;
		int radius;
		Point pos;
		void (*effect)(Player*);
	}PowerUp;

	extern double _sin(int angle);
	extern double _cos(int angle);

	void get_key_up(uint8_t);
	void get_key_down(uint8_t);
	char draw_into_board(uint32_t pn,Point p);
	void exit_game();

	int getRandIndex(int max);
	int getAvailablePowerupIndex();
	int square(int n);

	void managePowerups();

	// Powerup effects
	void powerUp_cleanScreen();
	
#endif