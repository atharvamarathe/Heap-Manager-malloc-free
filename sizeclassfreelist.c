#include "sizeclassfreelist.h"
#include <stdio.h>

sizeClass_free_list sizeClassFreeList[NUM_OF_CLASSES];


// initSizeClassFreeList : Initializes the sizeclass freelist when it is called for th first time
void initSizeClassFreeList() {

    for(int i =0;i<NUM_OF_CLASSES;i++) {

        for(int j = 0;j<SIZECLASS_FREE_LIST_SIZE;j++) {

            sizeClassFreeList[i].l[j].blockPtr = NULL;
            sizeClassFreeList[i].l[j].blockSize = 0;
            sizeClassFreeList[i].l[j].offset = -1;
        }
        sizeClassFreeList[i].rear = -1;
    }
    
}

/*
 * isSizeClassFreeListEmpty : to find whether the free-list is empty for a particular size-class 
 *                  Return  : TRUE - if it empty and FALSE if vice versa
 */

int isSizeClassFreeListEmpty(int sizeclass) {
        for(int j=0;j<SIZECLASS_FREE_LIST_SIZE;j++) {
            if(sizeClassFreeList[sizeclass].l[j].blockPtr != NULL && sizeClassFreeList[sizeclass].l[j].blockSize != 0) {
                return FALSE;
            }
        }
    return TRUE;
}


/*
 * removeBlockfromSizeClassFreeList : Removes a memory block from the free-list , usually called by myMalloc during 
 *                                    allocation of memory.
 *                           Params : ptr to the memory block and the size of the block 
 */


void removeBlockfromSizeClassFreeList(meta_data_block ptr, int size) {
    int i = 0;
    //finding the corresponding size-classes
    while(size > classSizeArray[i])
        i++;
    
    int j = 0;
    while(j<= sizeClassFreeList[i].rear && ptr != sizeClassFreeList[i].l[j].blockPtr) {
        j++;
    }
    if(j > sizeClassFreeList[i].rear) {
        perror("No block with the given pointer");
        return;
    }
    //swapping the memory block pointer wih the last element and then decreasing the rear position
    _sizeClass_free_list temp;
    temp = sizeClassFreeList[i].l[j];
    sizeClassFreeList[i].l[j] = sizeClassFreeList[i].l[sizeClassFreeList[i].rear];
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear] = temp;
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear].blockPtr = NULL;  
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear].blockSize = 0;
    sizeClassFreeList[i].rear -= 1;

}

/*
 * addBlocktoSizeClassFreeList : Adds memory block to the corresponding size-class freelist. Called by the free() function
 *                      Params : pointer to the memory block and the locatio of the memory block in the sizeclasslist
 */

void addBlocktoSizeClassFreeList(meta_data_block ptr, int size,int offset) {

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
    sizeClassFreeList[i].l[sizeClassFreeList[i].rear].offset = offset;
    return;
    
}

/*
 * removeAllFreeLitBlocksFromOffset : removes all the memory blocks from the list located at a particular page.
 *                                    Called by the free() function when the whole page is unused in order to return 
 *                                    it back to the kernel.
 *                           Params : sizeclass and offset i.e. the location of the page in the sizeclasslist.
 */

void removeAllFreeListBlocksFromOffset(int sizeclass, int offset) {

    int i=0;
    while(i<= sizeClassFreeList[sizeclass].rear) {
        if(sizeClassFreeList[sizeclass].l[i].offset == offset) {
            _sizeClass_free_list temp;
            temp = sizeClassFreeList[sizeclass].l[i];
            sizeClassFreeList[sizeclass].l[i] = sizeClassFreeList[sizeclass].l[sizeClassFreeList[sizeclass].rear];
            sizeClassFreeList[sizeclass].l[sizeClassFreeList[sizeclass].rear] = temp;
            sizeClassFreeList[sizeclass].l[sizeClassFreeList[sizeclass].rear].blockPtr = NULL;  
            sizeClassFreeList[sizeclass].l[sizeClassFreeList[sizeclass].rear].blockSize = 0;
            sizeClassFreeList[sizeclass].l[sizeClassFreeList[sizeclass].rear].offset = -1;
            sizeClassFreeList[sizeclass].rear -= 1;
        }
        i++;
    }
}

/*
 * getFreeBlockfromFreeList : returns memory block of the corresponding the sizeclass to the user. Called by malloc()
 *                            when freelist is not empty.
 *                   Params : number of bytes as requested from the user.
 */

meta_data_block getFreeBlockfromFreeList(size_t bytes) {

    int i = 0;
    while(i <= NUM_OF_CLASSES &&  bytes > classSizeArray[i])
        i++;
    meta_data_block ptr;
    ptr = sizeClassFreeList[i].l[0].blockPtr;
    //removing it from the list as it will be used by the user.
    removeBlockfromSizeClassFreeList(sizeClassFreeList[i].l[0].blockPtr,sizeClassFreeList[i].l[0].blockSize);
    return ptr;
}

