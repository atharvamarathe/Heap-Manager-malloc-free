#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <error.h>
#include <limits.h>
#include <stdint.h>
#include "mm.h"


static size_t SYSTEM_PAGE_SIZE = 0;


void * getPages(int units) {

    char *vm_page = mmap(
        0,units*SYSTEM_PAGE_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANON|MAP_PRIVATE,0,0);
    
    if(vm_page == MAP_FAILED) {

       // printf("Error! VM Page allocation failed \n");
        perror("Error ! VM Page allocation failed \n");
        return NULL;
    }
    memset(vm_page,0,units*SYSTEM_PAGE_SIZE);
    return (void *)vm_page;
}

void freePages(void* vm_page,int units) {

    if(munmap(vm_page,units*SYSTEM_PAGE_SIZE)) {

       // printf("Error! Could not munmap VM page to kernel");
       perror("Error! Could not munmap VM page to kernel");
    }
}

int isPageEmpty(meta_data_block head) {

    meta_data_block m1;
    m1=head;
    if(!m1)
        return INT_MIN;
    
    while(m1->nextBlock!=NULL) {

        if(m1->isFree == FALSE)
            return FALSE;
        m1 = m1->nextBlock;
    }

    return TRUE;

}




void mmInit() {

    SYSTEM_PAGE_SIZE = getpagesize();
    printf("VM Page Size = %lu\n",SYSTEM_PAGE_SIZE);
    meta_data_block a;
    a = (meta_data_block)getPages(1);
    if(!(a)) { 
        perror("getPages failed \n");
        return;
    }
    pageCount++;
    if(pageCount == MAX_PAGES)
        return;
    //TODO: Function to handle when all pages are exhausted
    pageList[pageCount-1].head =&a;
    pageList[pageCount-1].avaiableSize = SYSTEM_PAGE_SIZE - NUMBER_OF_BINS_PER_PAGE*METABLOCK_SIZE;
    createBinsList(pageList[pageCount-1].head);
    //    printf("VM Page Size = %lu\n",SYSTEM_PAGE_SIZE);
    // printf("page 1 = %p\n",*d1);
    // a->blockSize = SYSTEM_PAGE_SIZE-METABLOCK_SIZE;
    // a->isFree=TRUE;
    // a->nextBlock = d2->prevBlock = NULL;
    // a->offset = 0;

}

void initPageList() {

    for(int i = 0;i<MAX_PAGES;i++) {
        pageList[i].head = NULL;
        pageList[i].avaiableSize = 0;
    }
}

meta_data_block getFreeBlock(meta_data_block *head) {

    meta_data_block a;
    a= *head;
    if(head == NULL)
            printf("oh no  no\n");
    printf("HEllo\n");
    printf("the value %p\n",a->nextBlock);
    while(a->nextBlock != NULL && a->isFree != TRUE) {
        a = a->nextBlock;
    }
    return a;
    
}

void* Malloc(uint32_t size) {

    if(size <= 512) {

         if(pageCount == 0) {

            initPageList();
            mmInit();
        }
        int padding, bins_required;
        bins_required = size / BIN_SIZE;
        padding = size % BIN_SIZE;
        int j=0;
        while(j< MAX_PAGES && pageList[j].avaiableSize < size) {
            j++;
        }
        // if(j == MAX_PAGES)
            //DO something 

        meta_data_block d1,iter;
        d1 = getFreeBlock(pageList[j].head);
        iter = d1;
        for(int i=0;i<bins_required-1;i++) {

            if(mergeBins(iter)==INT_MIN) {
                // perror("Malloc failed as contigous memory not available");
                return NULL;
            }
            iter = iter->nextBlock;
        }

        if(padding != 0) {
            if(splitBins(iter->nextBlock,padding) == INT_MIN) {
                // perror("Malloc failed as memory blocks cannot be splited");
                return NULL;
            }
        }
        d1->blockSize = size;
        return (void *)d1+1;
    }
    return NULL;
}



void createBinsList(meta_data_block *head) {

    // mmInit();
    meta_data_block a,prev=NULL;
    a = *head;
    for(int i=0;i<NUMBER_OF_BINS_PER_PAGE-1;i++) {
        a->prevBlock = prev;
        a->blockSize=BIN_SIZE;
        a->nextBlock =(meta_data_block)(((char *)a)+METABLOCK_SIZE+BIN_SIZE);
        prev = a;
        a = a-> nextBlock;
    }
    //last block
    a->prevBlock = prev;
    a->blockSize=BIN_SIZE;
    a->nextBlock = NULL;
}


int splitBins(meta_data_block m1,int size) {

    meta_data_block mptr,nextptr=NULL;
    meta_data_block_ buf;
    mptr = m1;
    if(size > BIN_SIZE)
        return INT_MIN;
    if(mptr->nextBlock!=NULL && mptr->nextBlock->isFree==TRUE) {
        // buf = mptr->nextBlock;
        // // mptr->blockSize = size;
        // mptr->nextBlock = (((char *)mptr) + METABLOCK_SIZE + size);
        // nextptr = (meta_data_block)mptr -> nextBlock;
        // nextptr->isFree=TRUE;
        // nextptr->prevBlock = mptr;
        // // nextptr->blockSize = 2*BIN_SIZE - size;   //BIN_SIZE + (BIN_SIZE- size)
        // nextptr ->nextBlock = buf->nextBlock;
        // if(buf->nextBlock != NULL)
        //     buf->nextBlock->prevBlock = nextptr;
        nextptr = mptr->nextBlock;
        buf.blockSize = nextptr->blockSize;
        buf.isFree = nextptr->isFree;
        buf.nextBlock = nextptr->nextBlock;
        buf.offset = nextptr->offset;
        buf.prevBlock = nextptr->prevBlock;
        mptr->nextBlock =(meta_data_block) (((char *)mptr) + METABLOCK_SIZE + size);
        nextptr = (meta_data_block)mptr -> nextBlock;
        nextptr->prevBlock = buf.prevBlock;
        nextptr->nextBlock = buf.nextBlock;
        nextptr->isFree = buf.isFree;
        nextptr->offset = buf.offset;
        return 1;
    }
    return INT_MIN;
}

int mergeBins(meta_data_block m1) {

    if(!m1 || m1->nextBlock ==NULL || m1->isFree==FALSE || m1->nextBlock->isFree == FALSE)
        return INT_MIN;
    meta_data_block a;
    a = m1->nextBlock;
    // m1->blockSize +=  METABLOCK_SIZE + a ->blockSize;
    m1->nextBlock = a->nextBlock;
    a->prevBlock = m1;
    
    return 1;
}



void Free(void *ptr) {

    meta_data_block mptr;
    mptr = ((meta_data_block)ptr)-1;
    if(mptr==NULL) {
        perror("Can't free NULL Pointer");
        return;
    }

    mptr->isFree = TRUE;
    if(isPageEmpty(*(pageList[pageCount-1].head)) == TRUE) {
        freePages(pageList[pageCount-1].head,1);
        pageList[pageCount-1].head = NULL;
        pageList[pageCount-1].avaiableSize = 0;
        pageCount--;
        return;
    }

    if(mptr->nextBlock && mptr->nextBlock->isFree == TRUE) {

        mergeBins(mptr);
        int nextBlocksize = mptr->nextBlock->blockSize;
        mptr->blockSize += METABLOCK_SIZE + nextBlocksize;
    }

    if(mptr->prevBlock && mptr->prevBlock->isFree == TRUE) {
        meta_data_block prevblock=mptr->prevBlock;
        int prevBlocksize = mptr->prevBlock->blockSize;
        mergeBins(mptr->prevBlock);
       prevblock->blockSize += METABLOCK_SIZE + prevBlocksize;
    }
        
}




















