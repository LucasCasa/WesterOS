#include "graphic_manager.h"

static uint8_t* start = 0;

void draw_circle(Point* p, uint64_t radius, Color* c){
   signed int r = radius;
   for(signed int y=-r ; y<=r; y++){
      for(signed int x=-r; x<=r; x++){
         if(x*x+y*y <= r*r){
            setpixel(p->x + x*3 , p->y +y,c);
         }
      }
   }
}
void setpixel(int x ,int y, Color* c){
   start[x + y*SCR_WIDTH*BPP] = c->b;
   start[x+1 + y*SCR_WIDTH*BPP] = c->g;
   start[x+2 + y*SCR_WIDTH*BPP] = c->r;
}
void draw_text(Point*p,char* text){

}
void clear_screen(){
   for( int i = 0; i<SCR_WIDTH*SCR_HEIGHT*BPP;i++){
      start[i] = 0;
   }
}
/*void draw_image(Image g){

}*/
void set_graphic_pointer(){
   start = (*(uint32_t*)0x5080);
}
