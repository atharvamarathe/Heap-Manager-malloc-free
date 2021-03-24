#ifndef FREELIST_H
#define FREELIST_H

#include "mm.h"

#define FREE_LIST_SIZE 100
#define GETLCHILD(i) (2*i+1)
#define GETRCHILD(i) (2*i+2)
#define GETPARENT(i) ((i-1)/2)

typedef struct _free_list {

    meta_data_block blockPtr;
    int blockSize;
}_free_list;

typedef struct free_list {
 
    _free_list list[FREE_LIST_SIZE];
    int rear;
    int size;

}free_list;

static free_list freeList;

void initFreeList(free_list *l1);
void swapListElements(free_list *l1,int i,int j);
int addBlocktoFreeList(free_list *l1,meta_data_block ptr);
void removeBlockfromFreeList(free_list *l1);
void heapifyFreeList(free_list *l1,int i);
int deleteBlockfromFreeList(free_list *l1, int size);
int sortFreeList(free_list *l1);
#endif