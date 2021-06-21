#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_SIZECLASSES 42

typedef uint64_t statusBits;
typedef struct meta_data_block_ {
    struct meta_data_block_ *headPtr;
}meta_data_block;

typedef struct PageHeader {
    statusBits  FreeBlocks[8];
    meta_data_block *headBlock;
}PageHeader;

typedef struct sizeClassList {
    PageHeader headptr;
    struct sizeClassList *next;
}sizeClassList;


typedef struct memoryHeap{
    sizeClassList classes[42];
}memoryHeap;

