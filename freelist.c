#include <stdio.h>
#include <stdlib.h>
#include "memoryallocator.h"
#include "freelist.h"
#include <limits.h>
#include <error.h>


free_list freeList;

// initFreeList : Initializes the freelist when called for the first time.
void initFreeList(free_list *l1) {

    l1->size = FREE_LIST_SIZE;
    l1->rear = -1;
    for(int i=0;i<FREE_LIST_SIZE;i++) {

        l1->list[i].blockPtr = NULL;
        l1->list[i].blockSize = 0;
    }
    return;
}

// swapListElements : Helper function to swap two nodes of a heap 
void swapListElements(free_list *l1,int i,int j) {

    _free_list temp;
    temp = l1->list[j];
    l1->list[j] = l1->list[i];
    l1->list[i] = temp;
    return;
}

/*
 * addBlocktoFreeList : adds memory block to the heap of freelist. Returns 1 on success and INT_MIN upon failure.
 *             Params : ptr       : pointer to the memory block that needs to be added. 
 */


int addBlocktoFreeList(free_list *l1,meta_data_block ptr) {

    if(l1->rear == l1->size-1) {
        // perror("free_list size full");
        return INT_MIN;
    }
    l1->rear++;
    l1->list[l1->rear].blockPtr = ptr;
    l1->list[l1->rear].blockSize = ptr->blockSize;
    
    int i=l1->rear;

    while(i>0 && ptr->blockSize > l1->list[GETPARENT(i)].blockSize) {
        swapListElements(l1,i,GETPARENT(i));
        i= GETPARENT(i);
    }
    return 1;
}

/*
 * heapifyFreeList : Helper function to perform the heapify operation in the freelist heap
 */

void heapifyFreeList() {


    for(int i=0;i<freeList.rear;i++) {

        if(freeList.list[i].blockSize > freeList.list[GETPARENT(i)].blockSize) {
            int j =i;

            while(freeList.list[j].blockSize > freeList.list[GETPARENT(j)].blockSize) {
                swapListElements(&freeList,j,GETPARENT(j));
                j= GETPARENT(j);
            }
        }
    }
}


/*
 * removBlockfromFreeList : Works as the remove functionlality of the heap i.e. removes the first element from the list
 */
void removeBlockfromFreeList(free_list *l1) {

    if(l1->rear == -1)
        return;
    _free_list x = l1->list[0];
    l1->list[0] = l1->list[l1->rear];
    l1->list[l1->rear] = x;
    l1->rear--;
    int i=0;
    int max = 0;
    while(i<l1->rear) {

        int left = GETLCHILD(i);
        int right = GETRCHILD(i);

        if(left > l1->rear)
            return;
        if(right > l1->rear) {
            if(l1->list[i].blockSize < l1->list[left].blockSize)
                swapListElements(l1,i,left);
            return;
        }
        if(l1->list[left].blockSize > l1->list[right].blockSize)
            max = left;
        else
            max = right;
        swapListElements(l1,i,max);
        i=max;
    }

    return;
}



/*
 * deleteBlockfromFreeList : used to remove any element from the freelist. Called by malloc() while allocating memory
 *                         the freelist.
 *                 Params :m1 : pointer to the memory block
 *                       size : size of the memory block
 * Returns : 1 if success or INT_MIN on failure.
 */

int deleteBlockfromFreeList(free_list *l1,int size , meta_data_block m1) {

    if(!l1 || l1->list== NULL) {
        return INT_MIN;
    }
    int i=0;
    while(i < FREE_LIST_SIZE && l1->list[i].blockSize != size && l1->list[i].blockPtr != m1) 
        i++;
    if(i>= FREE_LIST_SIZE) {
        // perror("data block  with the given size not present");
        return INT_MIN;
    }
    l1->list[i] = l1->list[l1->rear];
    l1->list[l1->rear].blockPtr = NULL;
    l1->list[l1->rear].blockSize = 0;
    l1->rear--;
    heapifyFreeList();
    return 1;
}

// sortFreeList : helper function to sort the heap free list.
int sortFreeList(free_list *l1) {

    heapifyFreeList();
    for(int i = l1->rear;i>0;i++) {
        swapListElements(l1,0,i);

        int j = 0,index;

        do {
            index = GETLCHILD(j);
            if(l1->list[index].blockSize < l1->list[GETRCHILD(j)].blockSize && index < (i-1))
                i++;
            if(l1->list[j].blockSize < l1->list[index].blockSize && index < i)
                swapListElements(l1,j,index);
            j = index;
        }while(index < i);
    }
    return 1;
}


