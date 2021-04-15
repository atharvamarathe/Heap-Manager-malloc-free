#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <limits.h>
#include <stdint.h>
#include "sizeclasses.h"
#include "freelist.h"
#include "sizeclassfreelist.h"
#include "largealloc.h"


size_t SYSTEM_PAGE_SIZE = 0;


/*
 * getPages : Wrapper function to mmap() system call. 
 *          Returns Address of the virtual memory pages that are allocated. Returns NULL if mapping is failed 
 * Params : int units : number of virtual memory pages required.
 */


void * getPages(int units) {
    char *vm_page = mmap(
        NULL,units*SYSTEM_PAGE_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANON|MAP_PRIVATE,0,0);
    
    if(vm_page == MAP_FAILED) {

        perror("Error ! VM Page allocation failed \n");
        return NULL;
    }
    memset(vm_page,0,units*SYSTEM_PAGE_SIZE);
    return (void *)vm_page;
}

/*
 * freePages : Wrapper function to munmap() system call
 *             Returns the memory pages back to the kernel ( frees the page)
 * Params : vm_page : pointer to the memory pages 
 *          units : Number of pages that are to be unmapped.
 */

void freePages(void* vm_page,int units) {

    if(munmap(vm_page,units*SYSTEM_PAGE_SIZE)) {

       perror("Error! Could not munmap VM page to kernel");
    }
}

/*
 *
 * getFreeBlock : returns unused memory block from a virtual memory page
 * 
 * Params : Starting address of the virtual memory page
 */


meta_data_block getFreeBlock(meta_data_block head) {

    meta_data_block a;
    a= head;
    while(a->nextBlock != NULL && a->isFree == FALSE) {
        a = a->nextBlock;
    }
    return a;
    
}

/*
 * myMalloc : Used for dynamic allocation of memory. Returns pointer to the requested amount of memory
 * Params : number of bytes required by the user.
 */

void*  myMalloc(size_t bytes) {

    static int isInit = FALSE;

// Initializing the size-classes and free-lists

    if(isInit == FALSE) {
        SYSTEM_PAGE_SIZE = getpagesize();
        initSizeClassList();
        LargeAllocInit();
        initSizeClassFreeList();
        initFreeList(&freeList);
        isInit = TRUE;
        
    }

    if(bytes <= 0) {

        return NULL;
    }

// If the memory requirement is less than 1kB ( for size-classes)
    if(bytes < CLASS_SIZE_LIMIT) {
        int sizeclass=0;
        while(bytes > classSizeArray[sizeclass])
            sizeclass++;
// If freed blocks are available in freeList return address from the list 

        if(isSizeClassFreeListEmpty(sizeclass) == FALSE) {
            return (void *)(getFreeBlockfromFreeList(bytes)+1);
        }


        int i=0;
        while(bytes > classSizeArray[i]) {
            i++;
        }
        meta_data_block ptr,mptr;
// Getting page from the sizeclassList which has atleast one block avaiable for use
        ptr = getPageforAllocation(i);
        int k=0;
        while(k<MAX_PAGES && sizeClassList[i][k].head != ptr)
            k++;
        if(ptr == NULL) {
            // perror("Malloc failed \n");
            return NULL;
        }

        mptr = getFreeBlock(ptr);
        mptr->isFree = FALSE;
        mptr->headPtr = ptr;
        return (void *)(mptr+1);
    }
    else {
// if memory requirement is greatr than 1 kB (large Allocation)

// Checking if memory block is available in the free list 
//    Sorting the heap of free list containing pointer to memory blocks 
        sortFreeList(&freeList);
        int i = 0;
        while(i <= freeList.rear && bytes > freeList.list[i].blockSize)
            i++;
        if(i<= freeList.rear) {

            meta_data_block bptr;
// Spliting the memory block only if the remaining block will not cause hard memory fragmentation.

            if(freeList.list[i].blockSize > (bytes+METABLOCK_SIZE+CLASS_SIZE_LIMIT)) {
                if(splitLargeBlock(freeList.list[i].blockPtr,freeList.list[i].blockSize) == INT_MIN) {
                    return NULL;
                }
                addBlocktoFreeList(&freeList,freeList.list[i].blockPtr->nextBlock);
                bptr = freeList.list[i].blockPtr;
                deleteBlockfromFreeList(&freeList,freeList.list[i].blockSize,freeList.list[i].blockPtr);
                return (void *)(bptr+1);
            }
            bptr = freeList.list[i].blockPtr;
            deleteBlockfromFreeList(&freeList,freeList.list[i].blockSize,freeList.list[i].blockPtr);
            return (void *)(bptr+1);
        }
// If free-list do not contain any block , allocating new block in memory page.
        else {

            return returnLargeBlock(bytes);
        }
    }

    return NULL;
}



/*
 * myFree : function to free the dynamically allocated memory.
 */
void myFree(void *ptr) {
    meta_data_block mptr;
    mptr = ((meta_data_block)ptr)-1;
    if(mptr == NULL) {
        // perror("Can't free NULL pointer ");
        return ;
    }
    if(mptr->isFree == TRUE)
        return;
// Memory block belongs to size-classes

    if(mptr->blockSize < CLASS_SIZE_LIMIT) {

        meta_data_block headptr;
        headptr = mptr->headPtr;
        int sizeclass=0;
        while(sizeclass < NUM_OF_CLASSES && mptr->blockSize > classSizeArray[sizeclass])
            sizeclass++;

        static int isInit = FALSE;

        if(isInit == FALSE) {
        
            initSizeClassFreeList();
            isInit = TRUE;
        }
        int offset = 0;
        while(offset < MAX_PAGES && headptr != sizeClassList[sizeclass][offset].head) {
            offset++;
        }
        mptr->isFree = TRUE;
        if(isSizeClassPageEmpty(sizeclass,offset) == TRUE) {
        // if complete memory page is empty then unmap it to the kernel
            removeAllFreeListBlocksFromOffset(sizeclass,offset);
            removeEmptySizeClassPage(sizeclass,offset);
            return;
        }
        else {
            addBlocktoSizeClassFreeList(mptr,mptr->blockSize,offset);
    
        }


    }

    else {
// Memory block belongs to large alloc
        meta_data_block next = mptr->nextBlock;
        meta_data_block prev = mptr->prevBlock;
        meta_data_block head;
    // merging next and prev memory blocks if they are free
        if(next->isFree == TRUE) {
            mergeLargeBlock(mptr);

        }
        if(prev->isFree == TRUE) {
            mergeLargeBlock(prev);
            if(isLargeAllocPageEmpty(prev->headPtr)==TRUE) {
                // return page to the kernel if all blocks are free
                head = prev->headPtr;
                for(int i=0;i<=freeList.rear;i++) {

                    if(freeList.list[i].blockPtr->headPtr == head)
                        deleteBlockfromFreeList(&freeList,freeList.list[i].blockSize,freeList.list[i].blockPtr);
                }
                removeLargeAllocPage(head);
            }
            else {
                addBlocktoFreeList(&freeList,prev);
            }
        }
        else {

            if(isLargeAllocPageEmpty(mptr->headPtr)==TRUE) {
                head = mptr->headPtr;
                for(int i=0;i<=freeList.rear;i++) {
                    if(freeList.list[i].blockPtr->headPtr == head)
                        deleteBlockfromFreeList(&freeList,freeList.list[i].blockSize,freeList.list[i].blockPtr);
                }
                removeLargeAllocPage(head);
            }
            else 
                addBlocktoFreeList(&freeList,mptr);
        }

    }
    return;
}


/*
 * myCalloc : dynamically allocates memory and initializes it to zero 
 */

void* myCalloc(size_t num, size_t size) {

    void *ptr;

    ptr = myMalloc(num * size);
    if (ptr == NULL) {
        return ptr;
    }
// Initializing it to zero
    bzero(ptr, num * size);
    return ptr;

}

/*
 * myRealloc : Reallocates the dynamically allocated memory to a new size.
 *             frees the inital memory if size is zero.
 */

void* myRealloc(void* ptr, size_t size) {

    meta_data_block mptr;
    if(ptr == NULL)
        return myMalloc(size);
    mptr = ((meta_data_block)ptr)-1;
    if(size < 0)
        return NULL;
    if(size == 0) {
        myFree(ptr);
        return NULL;
    }

    void  *newPtr;
    newPtr = myMalloc(size);
    if(newPtr == NULL)
        return NULL;
// Moving contents to the memory to the new allocated location
    if(mptr->blockSize < size)
        memmove(newPtr,ptr,mptr->blockSize);
    else 
        memmove(newPtr,ptr,size);
    myFree(ptr);
    return newPtr;
}


