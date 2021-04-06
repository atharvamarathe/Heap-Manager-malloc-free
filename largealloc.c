#include <stdio.h>
#include "largealloc.h"
#include <limits.h>
#include <error.h>

large_alloc_list largeAllocList;




void LargeAllocInit() {

    for(int i =0;i<MAX_SIZE_FOR_LARGE_ALLOC;i++) {

       largeAllocList.largeBlock[i].ptr = NULL;
       largeAllocList.largeBlock[i].offset = 0;
       largeAllocList.largeBlock[i].remainingSize = 0;
    }
    largeAllocList.largeBlock[0].ptr = (meta_data_block)getPages(4);
    largeAllocList.largeBlock[0].offset  = 0;
    largeAllocList.largeBlock[0].remainingSize = SYSTEM_PAGE_SIZE - METABLOCK_SIZE;
    largeAllocList.count = 0;
    meta_data_block temp;
    temp = largeAllocList.largeBlock[0].ptr;
    temp->blockSize = SYSTEM_PAGE_SIZE-METABLOCK_SIZE;
    temp->prevBlock = NULL;
    temp->nextBlock = NULL;
    temp->isFree = TRUE;
    temp->headPtr = temp;
}

int splitLargeBlock(meta_data_block mptr, size_t bytes) {

    if(mptr->blockSize <= bytes) {
        return INT_MIN;
    }

    meta_data_block temp=NULL,next=NULL;
    size_t size = mptr->blockSize;
    temp = mptr;
    temp->blockSize = bytes;
    temp ->nextBlock = (meta_data_block)(((char *)temp) + METABLOCK_SIZE + bytes);

    next = temp->nextBlock;
    next->prevBlock = temp;
    next->nextBlock = NULL;
    next->blockSize = (size - (bytes + METABLOCK_SIZE));
    next->isFree = TRUE;
    next->headPtr = temp->headPtr;
    return 1;

}


void* returnLargeBlock(size_t bytes) {

    int i=0;
    while(i<= largeAllocList.count && bytes <= largeAllocList.largeBlock[i].remainingSize)
        i++;

    if(i <= largeAllocList.count) {

        meta_data_block mptr;
        mptr = largeAllocList.largeBlock[i].ptr;
        while(mptr->blockSize < bytes) {
            mptr = mptr -> nextBlock;
        }

        int buf = mptr->blockSize % bytes;
        if(buf < (METABLOCK_SIZE+1024))
            return(void *)(mptr+1);
        else {

            if(splitLargeBlock(mptr,bytes) == INT_MIN) {
                // perror("Malloc failed\n");
                return NULL;
            }
            else {

                return (void *)(mptr+1);
            }
        }


    }
    else {
        largeAllocList.count+=1;
        if(largeAllocList.count >= MAX_SIZE_FOR_LARGE_ALLOC) {

            // perror("Memory full. Can't allocate ");
            return  NULL;
        }
        largeAllocList.largeBlock[largeAllocList.count].ptr = (meta_data_block)getPages(4);
        largeAllocList.largeBlock[largeAllocList.count].offset = 0;
        largeAllocList.largeBlock[largeAllocList.count].remainingSize = SYSTEM_PAGE_SIZE - METABLOCK_SIZE;
        meta_data_block temp,a;
        temp = largeAllocList.largeBlock[largeAllocList.count].ptr;
        temp->blockSize = bytes;
        temp->prevBlock = NULL;
        temp->nextBlock = (meta_data_block)((char *)temp+bytes+METABLOCK_SIZE);
        temp->isFree = FALSE;
        temp->headPtr =  temp;
     
        largeAllocList.largeBlock[largeAllocList.count].remainingSize -= (bytes+METABLOCK_SIZE);
        largeAllocList.largeBlock[largeAllocList.count].offset = METABLOCK_SIZE+bytes;

        a = temp -> nextBlock;
        a-> prevBlock = temp;
        a-> nextBlock = NULL;
        a-> blockSize = largeAllocList.largeBlock[largeAllocList.count].remainingSize;
        a->isFree = TRUE;
        a->headPtr =  temp;
        return (void*)(temp+1);
        }
}