#include <stdio.h>
#include "largealloc.h"


large_alloc_list largeAllocList;

void LargeAllocInit() {

    for(int i =0;i<MAX_PAGES_FOR_LARGE_ALLOC;i++) {

       largeAllocList.largeBlock[i].ptr = NULL;
       largeAllocList.largeBlock[i].offset = 0;
       largeAllocList.largeBlock[i].remainingSize = 0;
    }
    largeAllocList.largeBlock[0].ptr = (meta_data_block)getPages(1);
    largeAllocList.largeBlock[0].offset  = 0;
    largeAllocList.largeBlock[0].remainingSize = SYSTEM_PAGE_SIZE - METABLOCK_SIZE;
    largeAllocList.count = 0;
    meta_data_block temp;
    temp = largeAllocList.largeBlock[0].ptr;
    temp->blockSize = SYSTEM_PAGE_SIZE-METABLOCK_SIZE;
    temp->prevBlock = NULL;
    temp->nextBlock = NULL;
    temp->isFree = TRUE;
}


void* returnLargeBlock(size_t bytes) {

    if(bytes <= largeAllocList.largeBlock[largeAllocList.count].remainingSize) {



    }

    else {
        largeAllocList.count+=1;
        if(largeAllocList.count >= MAX_PAGES_FOR_LARGE_ALLOC) {

            // perror("Memory full. Can't allocate ");
            return  NULL;
        }
        largeAllocList.largeBlock[largeAllocList.count].ptr = (meta_data_block)getPages(1);
        largeAllocList.largeBlock[largeAllocList.count].offset = 0;
        largeAllocList.largeBlock[largeAllocList.count].remainingSize = SYSTEM_PAGE_SIZE - METABLOCK_SIZE;
        meta_data_block temp,a;
        temp = largeAllocList.largeBlock[largeAllocList.count].ptr;
        temp->blockSize = bytes;
        temp->prevBlock = NULL;
        temp->nextBlock = (meta_data_block)((char *)temp+bytes+METABLOCK_SIZE);
        temp->isFree = FALSE;
     
        largeAllocList.largeBlock[largeAllocList.count].remainingSize -= (bytes+METABLOCK_SIZE);
        largeAllocList.largeBlock[largeAllocList.count].offset = METABLOCK_SIZE+bytes;

        a = temp -> nextBlock;
        a-> prevBlock = temp;
        a-> nextBlock = NULL;
        a-> blockSize = largeAllocList.largeBlock[largeAllocList.count].remainingSize;
        a->isFree = TRUE;
        return (void*)(temp+1);
        }
}