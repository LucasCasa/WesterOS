#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__


#define BITS_IN_BYTE    8
#define BLOCK_SIZE      1000

#define FREE_BLOCK  0
#define ALLC_BLOCK  1

#include <stdint.h>

typedef struct {
  	char * address;
    uint64_t size;
    uint64_t last_alloc;
} MemoryMap;

void init_malloc();

void init_bitmap(MemoryMap * bitmap, uint64_t address, uint64_t memory_size);

void * alloc_block(MemoryMap * bitmap);

void free_block(MemoryMap * bitmap, void * address);

void *  malloc();

void free(void * address);
#endif