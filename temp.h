#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t statusBits;
typedef struct meta_data_block_ {

    struct meta_data_block_ *nextBlock;


    
         

}meta_data_block;

typedef struct PageHeader {

    statusBits  FreeBlocks;
    meta_data_block *headBlock;

}PageHeader;
typedef struct sizeClassList {
    Pageheader headptr;
    struct sizeClassList *next;

}sizeClassList;


typedef struct memoryHeap{
    sizeClassList classes[42];
}memoryHeap;

