#ifndef SCR_SAVER_H_
#define SCR_SAVER_H_



#include <stdint.h>
#include "video_manager.h"
#include "graphic_manager.h"
#include "sys_lib.h"
#include "sys.h"

typedef struct ScreenImage{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[105 * 105 * 3 + 1];
} ScreenImage;

void alien();
void show_screensaver();
void set_screensaver_image(ScreenImage* s);



#endif
