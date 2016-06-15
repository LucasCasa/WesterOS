#ifndef _GAME_H
#define _GAME_H

	#include "lib.h"
	#include "call80.h"

	#define RADIUS      4
	#define MAX_PLAYERS 6
	#define HOLE_SIZE   10
	#define MAX_DRAW    90
	#define COLOR_POS   50
	#define NUMBER_POS  180
	#define RIGHT_POS   310
	#define LEFT_POS    500
	#define INIT_SPEED 	1

	#define WIDTH 1024
	#define HEIGHT 768

	#define NUM_POWERUPS 2
	#define MAX_POWERUPS 3
	#define MIN_POWERUP_TIME 50
	#define MAX_POWERUP_TIME 500
	#define POWERUP_RADIUS 15
	#define MAX_ACTIVE_EFFECTS 10
	
	struct Player;	

	typedef struct Vector{
		double x;
		double y;
	}Vector;

	typedef struct Effect{
		int active;
		int time_left;
		void (*final_effect)(struct Player*);
	}Effect;

	typedef struct Player{
		int id;
		int alive;
		Point pos;
		Vector acum;
		double speed;
		int radius;
		int angle;
		int mod;
		int time_with_inv;
		int time_no_inv;
		int erasable;
		Effect effects[MAX_ACTIVE_EFFECTS];
	}Player;

	typedef struct PowerUp{
		int id;
		int active;
		int radius;
		Point pos;
		void (*initial_effect)(Player*);
		void (*final_effect)(Player*);
		Color color;
	}PowerUp;

	extern double _sin(int angle);
	extern double _cos(int angle);

	void get_key_up(uint8_t);
	void get_key_down(uint8_t);
	char draw_into_board(uint32_t pn,Point p);
	void exit_game();
	void init_players(int nplayers);
	int lobby();

	int getRandIndex(int max);
	int getAvailablePowerupIndex();
	int getAvailableEffectIndex(Player*);
	int square(int n);

	void collide_powerup(PowerUp * pwup, Player * player);
	void managePowerups();
	void nothing(Player * p);
	void checkEffects(Player*);

	// Powerup effects
	void powerUp_cleanScreen(Player *);
	void powerUp_speed(Player *);
	void powerUp_speed_end(Player *);
	
#endif