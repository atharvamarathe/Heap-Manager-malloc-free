#ifndef SIZECLASS_FREELIST_H
#define SIZECLASS_FREELIST_H

#include "sizeclasses.h"

#define SIZECLASS_FREE_LIST_SIZE 100
typedef struct _sizeClass_free_list {

    meta_data_block blockPtr;
    int blockSize;
}_sizeClass_free_list;

typedef struct sizeClass_free_list {

    _sizeClass_free_list l[SIZECLASS_FREE_LIST_SIZE];
    int rear;
}sizeClass_free_list;

static sizeClass_free_list sizeClassFreeList[NUM_OF_CLASSES];

void initSizeClassFreeList();
void addBlocktoSizeClassFreeList(meta_data_block ptr, int size);
meta_data_block getFreeBlockfromFreeList(size_t bytes);


#endif