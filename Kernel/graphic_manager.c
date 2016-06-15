#include "graphic_manager.h"

extern Color background_color;
erasable_circle buffer[10];
int next = 0;
uint64_t st = 0xD000000;
static Image* font = (Image*)0x800000;

static uint8_t* start = 0;

void draw_circle(Point* p, uint64_t radius, Color* c){
   signed int r = radius;
   for(signed int y=-r ; y<=r; y++){
      for(signed int x=-r; x<=r; x++){
         if(x*x+y*y <= r*r){
            setpixel(p->x*3 + x*3 , p->y +y,c);
         }
      }
   }
}
void setpixel(int x ,int y, Color* c){
  if(x>=0 && x<SCR_WIDTH*3 && y>=0 && y<SCR_HEIGHT){
   start[x + y*SCR_WIDTH*BPP] = c->b;
   start[x+1 + y*SCR_WIDTH*BPP] = c->g;
   start[x+2 + y*SCR_WIDTH*BPP] = c->r;
 }
}

void draw_text(Point*p,char* text){
  int i = 0;
  while(text[i] != 0){
    draw_letter(p->x + LETTER_WIDTH*i,p->y,text[i]);
    i++;
  }
}
void draw_letter(int x, int y, char c){
  int totaloffset = x*BPP + y*SCR_WIDTH*BPP;
	int fontoffset = 0;
	uint8_t* startf;
	if(c < 32){
	 startf = font->pixel_data;
	}else{
	 startf = font->pixel_data + 21*(c - 32)*3;
	}
	for(int i = 0; i<LETTER_HEIGHT;i++){
		for(int j = 0; j<LETTER_WIDTH*BPP;j++){
			start[totaloffset + j] =startf[fontoffset + j + 2 ] ;
			j++;
			start[totaloffset + j] = startf[j + fontoffset];
			j++;
			start[totaloffset + j] = startf[j-2 + fontoffset];
		}
		totaloffset += SCR_WIDTH*BPP;
		fontoffset += font->width*BPP;
	}
}
int draw_erasable_circle(Point* p, uint64_t radius, Color* c){
  signed int r = radius;
  buffer[next].start = st;
  st+= 0x10000;
  buffer[next].radius = radius;
  buffer[next].x = p->x;
  buffer[next].y = p->y;
  buffer[next].used = 1;
  int i = 0;
  for(signed int y=-r ; y<=r; y++){
     for(signed int x=-r; x<=r; x++){
       buffer[next].start[i++] = start[p->x*3 + x*3 + (p->y +y)*SCR_WIDTH*BPP];
       buffer[next].start[i++] = start[p->x*3 + x*3 + 1 + (p->y +y)*SCR_WIDTH*BPP];
       buffer[next].start[i++] = start[p->x*3 + x*3 + 2 + (p->y +y)*SCR_WIDTH*BPP];
        if(x*x+y*y <= r*r){
           setpixel(p->x*3 + x*3 , p->y +y,c);
        }
     }
  }
  int ret = next;
  find_new_next();
  return ret;
}
void find_new_next(){
  int i = 0;
  while(buffer[next].used != 0 && i < 10){
    i++;
    next++;
    if(next == 10){
      next = 0;
    }
  }
  if(i == 0){
    //ERROR
  }
}
void undraw_erasable_circle(int id){
  signed int r = buffer[id].radius;
  int px = buffer[id].x;
  int py = buffer[id].y;
  char* bstart = buffer[id].start;
  int i = 0;
  for(signed int y=-r ; y<=r; y++){
     for(signed int x=-r; x<=r; x++){
       start[px*3 + x*3 + (py + y)*SCR_WIDTH*BPP] = bstart[i++];
       start[px*3 + x*3 + 1 + (py + y)*SCR_WIDTH*BPP] = bstart[i++];
       start[px*3 + x*3 + 2 + (py + y)*SCR_WIDTH*BPP] = bstart[i++];
     }
  }
  //free(buffer[id].start);
  buffer[id].used = 0;
}
void clear_screen(){
   for( int i = 0; i<SCR_WIDTH*SCR_HEIGHT*BPP;i++){
      start[i++] = background_color.b;
      start[i++] = background_color.g;
      start[i] = background_color.r;
   }
}
/*void draw_image(Image g){

}*/
void set_graphic_pointer(){
   start = (*(uint32_t*)0x5080);
   for(int i = 0; i<10;i++){
     buffer[i].used = 0;
   }
}
