#ifndef SIZECLASS_FREELIST_H
#define SIZECLASS_FREELIST_H

#include "sizeclasses.h"

#define SIZECLASS_FREE_LIST_SIZE 100


/*
 * Structure of _sizeclassFreeList :
 * Members : blockPtr    : Pointer to the memory block
 *           blockSize   : Size of the memory block
 *           offset      : The page number where the memory block is stored.
 */

typedef struct _sizeClass_free_list {

    meta_data_block blockPtr;
    int blockSize;
    int offset;
}_sizeClass_free_list;


/*
 * Structure of sizeClassFreeList :
 * Members : l    : array of struct _sizeclass_free_list which contains pointers to the memory block of a size-class
 *           rear : position of the last element in the array (just like a queue)
 */

typedef struct sizeClass_free_list {

    _sizeClass_free_list l[SIZECLASS_FREE_LIST_SIZE];
    int rear;
}sizeClass_free_list;

extern sizeClass_free_list sizeClassFreeList[NUM_OF_CLASSES];

void initSizeClassFreeList();

meta_data_block getFreeBlockfromFreeList(size_t bytes);

void addBlocktoSizeClassFreeList(meta_data_block ptr, int size,int offset);

int isSizeClassFreeListEmpty(int sizeclass);

void removeBlockfromSizeClassFreeList(meta_data_block ptr, int size);

void removeAllFreeListBlocksFromOffset(int sizeclass, int offset);
#endif
