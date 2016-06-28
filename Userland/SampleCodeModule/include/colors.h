#ifndef _COLORS_H
#define _COLORS_H


typedef struct Image {
  uint32_t 	 width;
  uint32_t 	 height;
  uint32_t 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  uint8_t	 pixel_data[105 * 105 * 3];
}Image ;

#endif
