#include <stdio.h>
#include "largealloc.h"


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
}


void* returnLargeBlock(size_t bytes) {

    meta_data_block temp;
}