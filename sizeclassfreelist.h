#ifndef SIZECLASS_FREELIST_H
#define SIZECLASS_FREELIST_H

#include "sizeclasses.h"

#define SIZECLASS_FREE_LIST_SIZE 100
typedef struct _sizeClass_free_list {

    meta_data_block blockPtr;
    int blockSize;
    int offset;
}_sizeClass_free_list;

typedef struct sizeClass_free_list {

    _sizeClass_free_list l[SIZECLASS_FREE_LIST_SIZE];
    int rear;
}sizeClass_free_list;

extern sizeClass_free_list sizeClassFreeList[NUM_OF_CLASSES];

void initSizeClassFreeList();
// void addBlocktoSizeClassFreeList(meta_data_block ptr, int size);
meta_data_block getFreeBlockfromFreeList(size_t bytes);
void addBlocktoSizeClassFreeList(meta_data_block ptr, int size,int offset);
int isSizeClassFreeListEmpty(int sizeclass);
void removeBlockfromSizeClassFreeList(meta_data_block ptr, int size);
void removeAllFreeListBlocksFromOffset(int sizeclass, int offset);
#endif