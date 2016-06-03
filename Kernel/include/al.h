static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[11 * 7 + 1];
} a_l = {
  11, 7, 3,
{0,0,0,0,1,1,1,0,0,0,0,
0,0,0,1,1,0,1,1,0,0,0,
0,0,1,1,0,0,0,1,1,0,0,
0,1,1,0,0,0,0,0,1,1,0,
0,1,1,1,1,1,1,1,1,1,0,
0,1,1,0,0,0,0,0,1,1,0,
0,1,1,0,0,0,0,0,1,1,0}
};
