#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <stdint.h>
#include <stddef.h>


// Contains the definition of the functions malloc(), free(), calloc() and realloc() and the system calls to the kernel/OS

/* 
 *
 *  The definition of the structure meta_data_block.
 * Members :-
 *  isFree              : Integer to indicate whether the data block is given to the user or not.
 *  prevBlock,nextBlock : Pointers to the previous and next meta data blocks just as doubly linked list 
 * headPtr              : Pointer to the starting address of the corresponding memory page.
 */

typedef struct meta_data_block_ {

    int isFree;
    uint32_t blockSize;
    struct meta_data_block_ *prevBlock;
    struct meta_data_block_ *nextBlock;
    struct meta_data_block_ *headPtr;
}meta_data_block_;

typedef meta_data_block_ * meta_data_block;


#define METABLOCK_SIZE sizeof(meta_data_block_)
#define TRUE 1
#define FALSE 0
#define NUMBER_OF_BINS_PER_PAGE SYSTEM_PAGE_SIZE/(BIN_SIZE+METABLOCK_SIZE)

int isPageEmpty(meta_data_block head);

void * getPages(int units);

void freePages(void* vm_page,int units);

meta_data_block getFreeBlock(meta_data_block head);

void myFree(void *ptr);

void*  myMalloc(size_t bytes);

void*  myCalloc(size_t num,size_t size);

void* myRealloc(void* ptr, size_t size);

extern  size_t SYSTEM_PAGE_SIZE;

#endif
