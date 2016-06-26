#include "ipc.h"

FIFO_table table;
int current_pos;

void list_entry(int index);

int initIPC(){
	current_pos = 0;
	for(int i=0; i<TABLE_SIZE; i++){
		table[i].fd = 0;
	}
	return 1;
}


int mkfifo(char * name){

	if(!validateName(name))
		return 0;

	// Advances current_pos to the next available slot -- If none is found return 0
	for(int condom = 0; current_pos<TABLE_SIZE+1; current_pos++, condom++){
		if(current_pos==TABLE_SIZE){
			current_pos = 0;
		}
		if(condom == TABLE_SIZE)
			return 0;
		if(!table[current_pos].fd){
			break;
		}
	}

	table[current_pos].fd = current_pos + OFFSET;
	copyName(table[current_pos].name, name, NAME_SIZE);
	table[current_pos].addr = malloc();
	table[current_pos].addr[0] = 0;
	return table[current_pos].fd;
}

int openfifo(char * name){
	for(int i=0; i<TABLE_SIZE; i++){
		if(!strcmp(table[i].name, name)){
			return table[i].fd;
		}
	}
	return 0;
}

int closefifo(int fd){
	int i = entryIndex(fd);
	if(i<0)
		return 0;
	table[i].fd = 0;
	free(table[current_pos].addr);
	return 1;
}


int writefifo(int fd, void * msg, int size){ // ret -1 on error (fifo not exists (?))
	int i = entryIndex(fd);
	if(i<0 || size>BLOCK_SIZE)
		return 0;
	memcpy(table[i].addr, msg, size);
	return 1;
}

int readfifo(int fd, void * buf, int size){ // returns message length; size is the buf size (max read amount)
	int i = entryIndex(fd),j;
	if(i<0)
		return 0;
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

void list_ipc(){
	print_message("\n\n********************************************\n",0xFF);
	print_message("******* List of currently open IPC's *******\n",0xFF);
	print_message("********************************************\n\n",0xFF);
	for(int i=0; i<TABLE_SIZE; i++){
		if(table[i].fd != 0)
			list_entry(i);
	}
}

void list_entry(int index){
	print_message("File descriptor: ",0xFF);
	print_number(table[index].fd);
	print_message(" - Name: ",0xFF);
	print_message(table[index].name,0xFF);
	print_message("\nContent: ",0xFF);
	print_message((char*)table[index].addr,0xFF);
	print_message("\n--------------------------------------------\n",0xFF);
}
