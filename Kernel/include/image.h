#ifndef _IMAGE_H
#define _IMAGE_H

#define LETTER_WIDTH    /*18*/12
#define LETTER_HEIGHT   /*24*/9

typedef struct Image {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[3801 * 41 * 3 + 1];
}Image ;

#endif
