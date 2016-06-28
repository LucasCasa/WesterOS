//Esto aloca memoria
#include <stdint.h>
#include "allocator.h"
#include "sys_lib.h"

static MemoryMap bm;

void init_bitmap(MemoryMap * bitmap, uint64_t address,uint64_t memory_size);

uint64_t get_free_block(MemoryMap * bitmap,int cantidad);

int get_block_status(MemoryMap * bitmap, int bit);

void set_block_status(MemoryMap * bitmap, uint64_t block, int status,int cant);

void * alloc_block(MemoryMap * bitmap, int cantidad);

void free_block(MemoryMap * bitmap, void * address);

static uint64_t address =0x10000000;

void
init_malloc(){
    init_bitmap(&bm,address,0x1000000); //16 mb de heap
    return;
}

void *
malloc(uint64_t size){
    int cantidad = (int)(size / BLOCK_SIZE);
    return alloc_block(&bm,cantidad+1);
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
        bitmap->address[i+(bitmap->size)] = 0; //pongo todos en 0 para que no haya paginas asociadas a el
}

void *
alloc_block(MemoryMap * bitmap, int cantidad){
    uint64_t free_block = get_free_block(bitmap,cantidad);
    if (free_block == -1)
        return 0; //no hay lugar
    int aux = (free_block-address-BLOCK_SIZE)/(BLOCK_SIZE);

    set_block_status(bitmap, aux,1,cantidad);

    return (void *)(free_block);
}

void
free_block(MemoryMap * bitmap, void * address){
    uint64_t block = (address-address-BLOCK_SIZE) / BLOCK_SIZE;
    int cantidad = bitmap->address[block+bitmap->size];
    set_block_status(bitmap, block, 0,cantidad);
}

uint64_t
get_free_block(MemoryMap * bitmap,int cantidad){
    int first_search = 1;
    uint64_t byte = bitmap->last_alloc;

    while (byte != bitmap->last_alloc || first_search == 1) {
        int j = 0;
        for(;j<cantidad;){
            if (bitmap->address[byte] != 0xFF) {
                int status = get_block_status(bitmap,byte);
                if (status == FREE_BLOCK && (j= cantidad +1)) {
                    bitmap->last_alloc = byte;
                    bitmap->address[byte+bitmap->size] = cantidad;
                    return address + (BLOCK_SIZE*(byte + 1));
                }
                if(status == FREE_BLOCK){
                    j++;
                }else{
                    j=0;
                }
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
set_block_status(MemoryMap * bitmap, uint64_t block, int status,int cant){
    for(int i = block; i<block + cant;i++){
        bitmap->address[i]=status;
    }
    return;
}
