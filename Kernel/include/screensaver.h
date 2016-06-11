#ifndef SCR_SAVER_H_
#define SCR_SAVER_H_



#include <stdint.h>
#include "video_manager.h"
#include "sys_lib.h"
#include "sys.h"

void alien();
void show_screensaver();

typedef struct TarImage{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[200 * 201 * 3 + 1];
} TarImage;

#endif
