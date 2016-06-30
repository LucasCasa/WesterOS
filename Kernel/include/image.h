#ifndef _IMAGE_H
#define _IMAGE_H

#define LETTER_WIDTH    /*18*/12
#define LETTER_HEIGHT   /*24*/9

typedef struct FontImage {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[1997 * 24 * 3 + 1];
}FontImage ;

typedef struct ScreenImage{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[105 * 105 * 3 + 1];
} ScreenImage;

#endif
