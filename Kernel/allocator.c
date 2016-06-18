
//Esto aloca memoria
#include <stdint.h>
#include "allocator.h"
#include "sys_lib.h"

static MemoryMap bm;

void init_bitmap(MemoryMap * bitmap, uint64_t address,uint64_t memory_size);

uint64_t get_free_block(MemoryMap * bitmap);

int get_block_status(MemoryMap * bitmap, int bit);

void set_block_status(MemoryMap * bitmap, uint64_t block, int status);

void * alloc_block(MemoryMap * bitmap);

void free_block(MemoryMap * bitmap, void * address);

static uint64_t address =0x10000000;

void
init_malloc(){
    init_bitmap(&bm,0x10000000,0x1000000); //16 mb de heap
    return;
}

void * 
malloc(){
  return alloc_block(&bm);
}

void
free(void * address){
  free_block(&bm,address);
}

void
init_bitmap(MemoryMap * bitmap, uint64_t address,uint64_t memory_size){
    bitmap->address = (void *)address;
    bitmap->size = (memory_size / BLOCK_SIZE) +1; //cuantos bloques va a haber
    bitmap->last_alloc = 0; //cual es la ultima posicion que se alloco

    int i;
    for (i = 0; i < bitmap->size; i++)
        bitmap->address[i] = 0; //inicializo todos con cero, para que esten todos available para allocar
}

void *
alloc_block(MemoryMap * bitmap){
    uint64_t free_block = get_free_block(bitmap);
    if (free_block == -1)
        return 0; //no hay lugar
    int aux = (free_block-address-BLOCK_SIZE)/(BLOCK_SIZE);

    set_block_status(bitmap, aux,1);

    return (void *)(free_block);
}

void
free_block(MemoryMap * bitmap, void * address){
    uint64_t block = (address-address-BLOCK_SIZE) / BLOCK_SIZE;
    set_block_status(bitmap, block, 0);
}

uint64_t
get_free_block(MemoryMap * bitmap){
    int first_search = 1;
    uint64_t byte = bitmap->last_alloc, bit;

    while (byte != bitmap->last_alloc || first_search == 1) {
        if (bitmap->address[byte] != 0xFF) {
            bit = 0;

            while (bit < BITS_IN_BYTE) {
                int status = get_block_status(bitmap,byte);

                if (status == FREE_BLOCK) {
                    bitmap->last_alloc = byte;
                    return address + (BLOCK_SIZE*(byte + 1));
                }

                bit++;
            }
        }

        byte = (byte + 1) % bitmap->size; //voy recorriento la tabla en forma circular
        first_search = 0;
    }

    return -1;
}

int
get_block_status(MemoryMap * bitmap,int byte){
    return bitmap->address[byte];
}

void
set_block_status(MemoryMap * bitmap, uint64_t block, int status){
    bitmap->address[block]=status;
    return;
}


