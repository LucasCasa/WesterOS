#include "clean_buffer.h"

 char Cbuffer[KB_SIZE + 1];

 char actual_pos = 0;
 char last_pos = 0;
 char Csize = 0;



void clean_set_char(char c){
		Cbuffer[(int)last_pos++] = c;
		Csize++;
	if( last_pos == KB_SIZE + 1)
		last_pos = 0;
}

char clean_size(){
	return Csize;
}

char clean_get_char(){
	char c;

	while(C_is_empty()) {}

	c = Cbuffer[(int)actual_pos++];

	if( actual_pos == KB_SIZE + 1)
		actual_pos = 0;

	if(Csize > 0) 
		Csize--;
	return c;
}

char C_is_empty(){
	return (actual_pos == last_pos);
}