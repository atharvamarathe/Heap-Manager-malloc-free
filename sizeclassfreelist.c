#include "sizeclassfreelist.h"
#include <stdio.h>

sizeClass_free_list sizeClassFreeList[NUM_OF_CLASSES];

void initSizeClassFreeList() {

    

    for(int i =0;i<NUM_OF_CLASSES;i++) {

        for(int j = 0;j<SIZECLASS_FREE_LIST_SIZE;j++) {

            sizeClassFreeList[i].l[j].blockPtr = NULL;
            sizeClassFreeList[i].l[j].blockSize = 0;
        }
        sizeClassFreeList[i].rear = -1;
    }
    
}

int isSizeClassFreeListEmpty(int sizeclass) {
        for(int j=0;j<SIZECLASS_FREE_LIST_SIZE;j++) {
            if(sizeClassFreeList[sizeclass].l[j].blockPtr != NULL && sizeClassFreeList[sizeclass].l[j].blockSize != 0)
                return FALSE;
        }
    return TRUE;
}

void removeBlockfromSizeClassFreeList(meta_data_block ptr, int size) {

    int i = 0;
    while(size > classSizeArray[i])
        i++;
    int j = 0;
    while(j<= sizeClassFreeList[i].rear && ptr != sizeClassFreeList[i].l[j].blockPtr) {
        j++;
    }
    if(j > sizeClassFreeList[i].rear) {
        // perror("No block with the given pointer");
        return;
    }
    _sizeClass_free_list temp;
    temp = sizeClassFreeList[i].l[j];
    sizeClassFreeList[i].l[j] = sizeClassFreeList[i].l[sizeClassFreeList[i].rear];
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear] = temp;
    sizeClassFreeList[i].rear -= 1;

}

void addBlocktoSizeClassFreeList(meta_data_block ptr, int size) {

    int i = 0;
    while(size > classSizeArray[i])
        i++;
    
    if(sizeClassFreeList[i].rear >= SIZECLASS_FREE_LIST_SIZE-1) {
        // perror("free List full !");
        return ;
    }
    sizeClassFreeList[i].rear +=1;
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear].blockPtr = ptr;
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear].blockSize = size;

    return;
    
}

meta_data_block getFreeBlockfromFreeList(size_t bytes) {

    int i = 0;
    while(i <= NUM_OF_CLASSES &&  bytes > classSizeArray[i])
        i++;
    meta_data_block ptr;
    ptr = sizeClassFreeList[i].l[0].blockPtr;
    removeBlockfromSizeClassFreeList(sizeClassFreeList[i].l[0].blockPtr,sizeClassFreeList[i].l[0].blockSize);
    return ptr;
}