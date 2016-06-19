#include "sys_lib.h"
#include "allocator.h"

#define NAME_SIZE 32
#define TABLE_SIZE 32
#define BASE_MEMORY 0xA00000
#define OFFSET 666

typedef struct FIFO_entry{
	int fd;
	char name[NAME_SIZE];
	uint8_t * addr;
}FIFO_entry;

typedef FIFO_entry FIFO_table[TABLE_SIZE];

int mkfifo(char * name); // returns fd
int openfifo(char * name); // returns fd
int closefifo(int fd); // -1 error
int writefifo(int fd, void * msg, int size); // ret -1 on error (fifo not exists (?))
int readfifo(int fd, void * buf, int size); // returns message length; size is the buf size (max read amount)

void copyName(char * dest, char * origin, int max_size);
int validateName(char * name);
int entryIndex(int fd);