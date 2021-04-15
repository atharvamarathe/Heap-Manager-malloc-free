#include <stdio.h>
#include "largealloc.h"
#include <limits.h>
#include <error.h>

large_alloc_list largeAllocList;



// LargeAllocInit : Initializes the largealloclist when it is called for the first time.

void LargeAllocInit() {

    for(int i =0;i<MAX_SIZE_FOR_LARGE_ALLOC;i++) {

       largeAllocList.largeBlock[i].ptr = NULL;
       largeAllocList.largeBlock[i].offset = 0;
       largeAllocList.largeBlock[i].remainingSize = 0;
    }
    
}

/*
 * splitLargeBlock : Splits the memory block into two parts in such a way that one blocks has memory equal to 
 *                   the memory requested. Splits the blocks only when it will not result into hard fragmentation
 *           Params : mptr : the memory block that needs to be splitted.
 *                   bytes : the number of bytes required in the memory block after splitting.
 * Returns : 1 on success and INT_MIN on failure.
 */

int splitLargeBlock(meta_data_block mptr, size_t bytes) {

    if(mptr->blockSize <= bytes) {
        return INT_MIN;
    }

    meta_data_block temp=NULL,next=NULL,ptr=NULL;


    if(mptr->nextBlock != NULL)
        ptr = mptr->nextBlock->nextBlock;
    else 
        ptr = NULL;
    size_t size = mptr->blockSize;
    temp = mptr;
    temp->blockSize = bytes;
    temp ->nextBlock = (meta_data_block)(((char *)temp) + METABLOCK_SIZE + bytes);

    next = temp->nextBlock;
    next->prevBlock = temp;
    next->nextBlock = ptr;
    next->blockSize = (size - (bytes + METABLOCK_SIZE));
    next->isFree = TRUE;
    next->headPtr = temp->headPtr;
    return 1;

}

/*
 * mergeLargeBlock : merge two consecutive memory blocks together to form a single block. Called by the free() funcion
 *                   when two consecutive blocks are unused.
 *          Params : m1 : pointer to the inital memory block. Its next block will be merged with it.
 * Returns 1 on Succuess and INT_MIN on failure.
 */
int mergeLargeBlock(meta_data_block m1) {

    if(!m1 || m1->nextBlock ==NULL || m1->isFree==FALSE || m1->nextBlock->isFree == FALSE)
        return INT_MIN;
    meta_data_block a;
    a = m1->nextBlock;
    size_t nextSize = a->blockSize;
    m1->nextBlock = a->nextBlock;
    a->nextBlock->prevBlock = m1;
    m1->blockSize += METABLOCK_SIZE + nextSize;
    return 1;
}

/*
 * returnLargeBlock : returns pointer to the memory block which has a size greater than or equal to the size available 
 *                    in the page. If no block is available then new block is allocated by allocating new page.
 *           Params : no of bytes the user wants.
 */

void* returnLargeBlock(size_t bytes) {

    int i=0,requiresPages;
    while(i<= largeAllocList.count && bytes <= largeAllocList.largeBlock[i].remainingSize)
        i++;
    if(i >= largeAllocList.count && largeAllocList.largeBlock[0].ptr == NULL && largeAllocList.largeBlock[0].remainingSize == 0 && largeAllocList.largeBlock[0].offset == 0) {
        // if largeAlloclist is empty then new page is allocated according o the requirement.
        requiresPages = 4 > ((bytes+ METABLOCK_SIZE)/SYSTEM_PAGE_SIZE) ? 4 : ((bytes+ METABLOCK_SIZE)/SYSTEM_PAGE_SIZE)+1;
        largeAllocList.largeBlock[0].ptr = (meta_data_block)getPages(requiresPages);
        largeAllocList.largeBlock[0].offset  = 0;
        largeAllocList.largeBlock[0].remainingSize = bytes;
        largeAllocList.count = 0;
        meta_data_block temp;
        temp = largeAllocList.largeBlock[0].ptr;
        temp->blockSize = bytes;
        temp->prevBlock = NULL;
        temp->nextBlock = NULL;
        temp->isFree = TRUE;
        temp->headPtr = temp;
        i=0;
    }
    if(i <= largeAllocList.count) {
        // Memory block with the required size is available
        meta_data_block mptr;
        mptr = largeAllocList.largeBlock[i].ptr;
        while(mptr->blockSize < bytes) {
            mptr = mptr -> nextBlock;
        }
        mptr->headPtr = largeAllocList.largeBlock[i].ptr;
        int buf = mptr->blockSize % bytes;
        if(buf < (METABLOCK_SIZE+1024))
            return(void *)(mptr+1);
        else {
            // spliting the block of memory only if it does not result into hard fragmentation.
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
        //no block of memory satisfy the request so allocate a new page for it 
        largeAllocList.count+=1;
        if(largeAllocList.count >= MAX_SIZE_FOR_LARGE_ALLOC) {

            // perror("Memory full. Can't allocate ");
            return  NULL;
        }
        requiresPages = 4 > ((bytes+ METABLOCK_SIZE)/SYSTEM_PAGE_SIZE) ? 4 : (bytes+ METABLOCK_SIZE)/SYSTEM_PAGE_SIZE;
        largeAllocList.largeBlock[largeAllocList.count].ptr = (meta_data_block)getPages(requiresPages);
        largeAllocList.largeBlock[largeAllocList.count].offset = 0;
        largeAllocList.largeBlock[largeAllocList.count].remainingSize = requiresPages*SYSTEM_PAGE_SIZE - METABLOCK_SIZE;
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

/*
 * isLargeAllocEmpty : to find whether the large allocated page is empty or not.
 *        Returns TRUE if all the blocks are free and returns FALSE if there is atleast one block which is in use.
 */

int isLargeAllocPageEmpty(meta_data_block head) {

    meta_data_block iter= head;
    while(iter!= NULL) {
        if(iter->isFree == FALSE)
            return FALSE;
        iter = iter -> nextBlock;
    }
    return TRUE;
}

/*
 * removeLargeAllocPage : unmaps the pages if they are empty. This function is called by free() when it detected that 
 *                         the whole page is empty.
 */

void removeLargeAllocPage(meta_data_block head) {

    int i =0;
    while(i < MAX_SIZE_FOR_LARGE_ALLOC && head != largeAllocList.largeBlock[i].ptr) {
        i++;
    }

    if(i==MAX_SIZE_FOR_LARGE_ALLOC)
        return;
    large_alloc temp  = largeAllocList.largeBlock[i];
    largeAllocList.largeBlock[i] = largeAllocList.largeBlock[largeAllocList.count];
    largeAllocList.largeBlock[largeAllocList.count].offset = 0;
    largeAllocList.largeBlock[largeAllocList.count].remainingSize = 0;
    largeAllocList.largeBlock[largeAllocList.count].ptr = NULL;
    largeAllocList.count-=1;
    freePages(head,4);

    return;
}
