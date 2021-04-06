#ifndef LARGE_ALLOC_H
#define LARGE_ALLOC_H

// Function to handle large memory requirements which are of the multiple of page size 
// For memory greater than page size it will be given in the multiple of page sizes only. For eg : if 6kB are required then it is rounded upto to 8kB size
// which is equivalent to 2 pages of memory.
#include "mm.h"

#define MAX_SIZE_FOR_LARGE_ALLOC 100

typedef struct _large_alloc {

    meta_data_block ptr;
    int offset;
    int remainingSize;
}large_alloc;

typedef struct _large_alloc_list {
    large_alloc largeBlock[MAX_SIZE_FOR_LARGE_ALLOC];
    int count;
}large_alloc_list;
extern large_alloc_list largeAllocList;
int splitLargeBlock(meta_data_block mptr, size_t bytes);
int mergeLargeBlock(meta_data_block m1);
void* returnLargeBlock(size_t bytes);
void LargeAllocInit();
int isLargeAllocPageEmpty(meta_data_block head);
void removeLargeAllocPage(meta_data_block head);











#endif