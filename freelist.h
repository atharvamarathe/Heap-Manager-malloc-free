#ifndef FREELIST_H
#define FREELIST_H

#include "largealloc.h"

#define FREE_LIST_SIZE 100
#define GETLCHILD(i) (2*i+1)
#define GETRCHILD(i) (2*i+2)
#define GETPARENT(i) ((i-1)/2)


/*
 * structure of _free_list :
 * Members : blockPtr  : The pointer to the memory block.
 *           blockSize : The size of the memory block.
 */
typedef struct _free_list {

    meta_data_block blockPtr;
    int blockSize;
}_free_list;

/*
 * Structure of freeList : Customized heap data structure for the implementation fo freelist.
 * Members : list : array of the struct _free_list
 *           rear : Pointing to the last element of the list 
 *           size : Maximum capacity of the heap array
 */

typedef struct free_list {
 
    _free_list list[FREE_LIST_SIZE];
    int rear;
    int size;

}free_list;

extern free_list freeList;

void initFreeList(free_list *l1);

void swapListElements(free_list *l1,int i,int j);

int addBlocktoFreeList(free_list *l1,meta_data_block ptr);

void removeBlockfromFreeList(free_list *l1);

void heapifyFreeList();

int deleteBlockfromFreeList(free_list *l1, int size, meta_data_block m1);

int sortFreeList(free_list *l1);




#endif
