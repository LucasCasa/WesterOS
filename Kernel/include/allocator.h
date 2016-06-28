#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__


#define BITS_IN_BYTE    8
#define BLOCK_SIZE      0x1000

#define FREE_BLOCK  0
#define ALLC_BLOCK  1

#include <stdint.h>

typedef struct {
  	char * address;
    uint64_t size;
    uint64_t last_alloc;
} MemoryMap;

void init_malloc();

uint64_t get_free_block(MemoryMap * bitmap,int cantidad);

int get_block_status(MemoryMap * bitmap, int bit);

void set_block_status(MemoryMap * bitmap, uint64_t block, int status,int cant);

void init_bitmap(MemoryMap * bitmap, uint64_t address, uint64_t memory_size);

void * alloc_block(MemoryMap * bitmap, int cantidad);

void free_block(MemoryMap * bitmap, void * add);

void *  malloc(uint64_t size);

void free(void * address);
#endif
