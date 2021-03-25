#ifndef MM_H
#define MM_H

#include <stdint.h>
#define TRUE 1
#define FALSE 0
#define METABLOCK_SIZE sizeof(meta_data_block_)
#define BIN_SIZE 16
#define NUMBER_OF_BINS_PER_PAGE SYSTEM_PAGE_SIZE/(BIN_SIZE+METABLOCK_SIZE)

typedef struct meta_data_block_ {

    int isFree;
    uint32_t blockSize;
    struct meta_data_block_ *prevBlock;
    struct meta_data_block_ *nextBlock;
    uint32_t offset;
}meta_data_block_;

typedef meta_data_block_ * meta_data_block;


typedef struct data_block_ {

    struct data_block_ *next;
    struct data_block_ *prev;
    char *data;
    meta_data_block metaData;
}data_block_;

typedef data_block_ * data_block;

// typedef struct page_list{

//     meta_data_block head;
//     struct page_list_ *next;
//     struct page_list_ *prev;

// }page_list;

// void mm_init(data_block *d1);
int isPageEmpty(meta_data_block head);
void * getPages(int units);
void freePages(void* vm_page,int units);
void mmInit();
void initPageList();
void* Malloc(uint32_t size);
void createBinsList(meta_data_block *head);
int splitBins(meta_data_block m1,int size);
int mergeBins(meta_data_block m1);
meta_data_block getFreeBlock(meta_data_block *head);
void Free(void *ptr);
// void freeDatablock(data_block d1);
// void pageInit(data_block *d1);
// static data_block d1;
static size_t SYSTEM_PAGE_SIZE = 0;
static size_t SYSTEM_PAGE_SIZE;
static int pageCount=0;
static int currentFreePage=0;


#endif
