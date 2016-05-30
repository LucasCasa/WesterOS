#ifndef SOUNDS_H
#define SOUNDS_H

#define A 11492 //G3S 
#define B 10847 //A3 
#define C 9121 //C4 
#define D 8609 //C4S 
#define E 8126 //D4 
#define F 7670 //D4S 
#define G 7239 //E4 
#define H 6833 //F4 
#define I 6449 //F4S  
#define J 6087 //G4 
#define K 5743 //G4S 
#define L 5423 //A 
#define M 5119 //A4S
#define N 4831 //B4 
#define O 4560 //C5 
#define P 4304 //C5S
#define Q 4063 //D5 
#define R 3834 //D5S
#define S 3619 //E5 
#define T 3416 //F5 
#define U 3224 //F5S
#define V 3043 //G5 
#define W 2873 //G5S
#define X 2711 //A5 
#define Y 2559 //A5S
#define Z 2416 //B5 


#define BECAUSE (uint8_t*) 0x500003
#define RICKROLL (uint8_t*) 0x50009D
 
 
#include <stdint.h>

void read_song(uint64_t songNum);
void playPiano(uint32_t frec);
void play_song2(uint8_t song, uint32_t tempo);

#endif
