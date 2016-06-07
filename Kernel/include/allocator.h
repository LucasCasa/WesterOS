#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__


#define BITS_IN_BYTE    8
#define BLOCK_SIZE      ((uint64_t) 4 * 1024) // 4kb

#define FREE_BLOCK  0
#define ALLC_BLOCK  1

#include <stdint.h>

typedef struct {
    char * address;
    void * offset;
    uint64_t size;
    uint64_t last_alloc;
} MemoryMap;

void init_bitmap(MemoryMap * bitmap, char * address, void * offset, uint64_t memory_size);

void * alloc_block(MemoryMap * bitmap);

void free_block(MemoryMap * bitmap, void * address);
#endif