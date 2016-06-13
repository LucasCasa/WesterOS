#ifndef _GRAPHIC_MANAGER_H_
#define _GRAPHIC_MANAGER_H_

#include "typedef.h"
#include "image.h"

#define SCR_WIDTH    1024
#define SCR_HEIGHT   768
#define BPP          3

typedef struct erasable_circle{
  int x;
  int y;
  char* start;
  int radius;
  int used;
}erasable_circle;

void draw_circle(Point* p, uint64_t radius, Color* c);
void draw_text(Point*p,char* text);
void clear_screen();
//void draw_image(Image g);
void setpixel(int x, int y, Color* c);


#endif
