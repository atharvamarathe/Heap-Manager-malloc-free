#include <stdio.h>
#include <stdlib.h>
#include "mm.h"

#define FREE_LIST_SIZE 30

typedef struct free_list{

    meta_data_block blockPtr;
    uint32_t size;
}free_list;

static free_list freeList[FREE_LIST_SIZE];


void initFreeList(free_list *head) {

    free_list *iter;
    iter = head;
    for(int i=0;i<FREE_LIST_SIZE;i++) {
        iter->blockPtr = NULL;
        iter->size = 0;
        iter+=1;
    }

    return;
}


void removeBlockfromFreeList(free_list *head,meta_data_block ptr) {

    ;
}
