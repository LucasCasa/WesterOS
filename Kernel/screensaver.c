#include "screensaver.h"
#define INTERVAL 50
double MASS = 1;
double RADIUS = 52;
TarImage* image = (TarImage*) 0x850000;
Point offset[10];
PointD mod[10];
static int next = 0;
int ttn = 1;
int time_to_reset = 1000;
void show_screensaver(){
	//clear_screen();
	reset_current_video();
	westeros();
}

int validateScreenTime(int time){
	if(time <= 0){		// Error for invalid value
		return 0;
	}
	if(time > MINSCRSAVERTIME){ // If the time is too big, it is set to a default value
		time = MINSCRSAVERTIME;
	}
	screensaver_time = time * 1000 / 55;
	return 1;
}
void westeros(){
	time_to_reset--;
	if(!time_to_reset){
		next = 0;
		ttn = 1;
		clear_screen();
	}
	int r = 0,g = 0, b = 0;
	uint8_t* st = (*(uint32_t*)0x5080);
	ttn--;
	if(ttn == 0 && next < 10){
		int valid = 0;
		int j = 0;
		while(!valid){
			valid = 1;
			offset[next].x = (rand() % 800) + 50;
			offset[next].y = (rand() % 600) + 50;
			for(int i = 0; i<next && valid ;i++){
				if(getDistance2(offset[i],offset[next]) < 4*(image->width/2)*(image->width/2)){
					valid = 0;
				}
			}
		}
		mod[next].x =(rand() % 10 - 5 )*2;
		mod[next].y = (rand() % 10 - 5)*2;
		next++;
		ttn = INTERVAL;
	}
	int totaloffset;
	for(int i = 0; i<next; i++){
		if(offset[i].y + image->height >= 768){
			mod[i].y = -1*mod[i].y;
		}else if(offset[i].y <= 0){
			mod[i].y = -1*mod[i].y;
		}
		if(offset[i].x + image->width >= 1024){
			mod[i].x = -1*mod[i].x;
		}else if(offset[i].x <= 0){
			mod[i].x = -1*mod[i].x;
		}
		offset[i].x+= mod[i].x;
		offset[i].y+= mod[i].y;
	}
	for(int i = 0; i<next;i++){
		for(int j = i+1; j<next;j++){
			if(getDistance2(offset[i],offset[j]) < 4*(RADIUS)*(RADIUS)){
				bounce(&(offset[i]),&(mod[i]),&(offset[j]),&(mod[j]));
			}
		}
		totaloffset = offset[i].y*1024*3 + offset[i].x*3;

		for(uint64_t i = 0; i<image->height;i++){
			for(uint64_t j = 0;j<image->width*3;j++){
				r = image->pixel_data[j + i*image->width*3];
				g = image->pixel_data[j+1 + i*image->width*3];
				b = image->pixel_data[j+2 + i*image->width*3];
				if(r || g || b){
				st[totaloffset + j + 1024*i*3] = b;
				j++;
				st[totaloffset + j + 1024*i*3] = g;
				j++;
				st[totaloffset + j + 1024*i*3] = r;
			}else{
				j+=2;
			}
			}
		}
	}
}
int getDistance2(Point p1, Point p2){
	return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y - p2.y);
}

void bounce(Point* p1, PointD* mod1,Point* p2, PointD* mod2) {
        double dx  = p2->x - p1->x;
        double dy  = p2->y - p1->y;
        double dvx = mod2->x - mod1->x;
        double dvy = mod2->y - mod1->y;
        double dvdr = dx*dvx + dy*dvy;
        double dist = RADIUS + RADIUS;
        double j = 2 * MASS * MASS * dvdr / ((MASS + MASS) * dist);
        double jx = j * dx / dist;
        double jy = j * dy / dist;
        mod1->x += jx / MASS;
        mod1->y += jy / MASS;
        mod2->x -= jx / MASS;
        mod2->y -= jy / MASS;
				p1->x+=mod1->x;
				p1->y+=mod1->y;
				p2->x+=mod2->x;
				p2->y+=mod2->y;
}

void alien(){
	print_message("############.`::::::::::::::::::::::::::::::::::::::::::::::::'.#############\n",0xFF);
	print_message("############# :::::'#######`::::::::::::::::::::'#######`::::: ##############\n",0xFF);
	print_message("############# ::::'##.S$S.##`::::::::::::::::::'##.S$S.##`:::: ##############\n",0xFF);
	print_message("############'.:::'##.$$$$$.##`::::::::::::::::'##.$$$$$.##`:::.`#############\n",0xFF);
	print_message("############ :::'##.$$$$$$$.##`::::::::::::::'##.$$$$$$$.##`::: #############\n",0xFF);
	print_message("############ :'##.$$$$$$$$$$$.##`::::::::::'##.$$$$$$$$$$$.##`: #############\n",0xFF);
	print_message("############ :.##`$$$$$$$$$$$$.##`::::::::'##.$$$$$$$$$$$$'##.: #############\n",0xFF);
	print_message("############ ::.##`$$$$$$$$$$$$.##`::::::'##.$$$$$$$$$$$$'##.:: #############\n",0xFF);
	print_message("############ :::.##`$$$$$$$$$$$$.##`::::'##.$$$$$$$$$$$$'##.::: #############\n",0xFF);
	print_message("############.`:::.##`$$$$$$$$$$$$.##`::'##.$$$$$$$$$$$$'##.:::'.#############\n",0xFF);
	print_message("############# ::::.##`S$$$$$$$$$$S.##::##.S$$$$$$$$$$S'##.:::: ##############\n",0xFF);
	print_message("############# :::::.#################::#################.::::: ##############\n",0xFF);
	print_message("#############.`::::::::::::::::::::::::::::::::::::::::::::::'.##############\n",0xFF);
	print_message("##############.`::::::::::::::::::::::::::::::::::::::::::::'.###############\n",0xFF);
	print_message("###############.`::::::::::::::::::::::::::::::::::::::::::'.################\n",0xFF);
	print_message("################.`::::::::::::::::::::::::::::::::::::::::'.#################\n",0xFF);
	print_message("#################.`::::::::::::::::::::::::::::::::::::::'.##################\n",0xFF);
	print_message("##################.`::::::::::::::::::::::::::::::::::::'.###################\n",0xFF);
	print_message("###################.`::::::::::::::::::::::::::::::::::'.####################\n",0xFF);
	print_message("####################.`::::::::::::::::::::::::::::::::'.#####################\n",0xFF);
	print_message("#####################.`::::::::::::::::::::::::::::::'.######################\n",0xFF);
	print_message("######################.`::::::::############::::::::'.#######################\n",0xFF);
	print_message("#######################.`:::::::############:::::::'.########################\n",0xFF);
	print_message("########################.`::::::::::::::::::::::::'.#########################\n",0xFF);
	print_message("#########################.`:::::::::::::::::::::::'.#########################\n",0xFF);
	print_message("##########################.`:::::::::::::::::::::'.##########################\n",0xFF);
}
