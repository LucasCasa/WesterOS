#include "ipc.h"

FIFO_table table;
int current_pos;

int initIPC(){
	current_pos = 0;
	for(int i=0; i<TABLE_SIZE; i++){
		table[i].fd = 0;
	}
}


int mkfifo(char * name){
	FIFO_entry entry;

	if(!validateName(name))
		return -1;

	for(int condom = 0; current_pos<TABLE_SIZE+1; current_pos++, condom++){
		if(current_pos==TABLE_SIZE){
			current_pos = 0;
		}
		if(condom == TABLE_SIZE)
			break;
		if(!table[current_pos].fd){
			entry = table[current_pos];
			break;
		}
	}
	entry.fd = current_pos + OFFSET;
	copyName(entry.name, name, NAME_SIZE);
	// ACA DEBERIA SER MALLOC
	entry.addr = BASE_MEMORY + BUFFER_SIZE * current_pos;
	return entry.fd;
}

int openfifo(char * name){
	for(int i=0; i<TABLE_SIZE; i++){
		if(!strcmp(table[i].name, name)){
			return table[i].fd;
		}
	}
	return -1;
}

int closefifo(int fd){
	int i = entryIndex(fd);
	if(i<0)
		return -1;
	table[i].fd = 0;
	return 1;
}


int writefifo(int fd, void * msg, int size){ // ret -1 on error (fifo not exists (?))
	int i = entryIndex(fd);
	if(i<0 || size>BUFFER_SIZE)
		return -1;
	memcpy(table[i].addr, msg, size);
	return 1;
}

int readfifo(int fd, void * buf, int size){ // returns message length; size is the buf size (max read amount)
	int i = entryIndex(fd),j;
	if(i<0)
		return -1;
	for(j=0; j<size-1 && table[i].addr[j]!=0; j++){
		((char*)buf)[j] = table[i].addr[j];
		table[i].addr[j] = 0;
	}
	return j;
}

void copyName(char * dest, char * origin, int max_size){
	int i;
	for(i=0; origin[i]!=0 && i<max_size-2; i++){
		dest[i] = origin[i];
	}
	dest[i] = 0;
}

int validateName(char * name){
	for(int i=0; i<TABLE_SIZE; i++){
		if(!strcmp(table[i].name,name))
			return 0;
	}
	return 1;
}

int entryIndex(int fd){
	for(int i=0; i<TABLE_SIZE; i++){
		if(table[i].fd == fd)
			return i;
	}
	return -1;
}
