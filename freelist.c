#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "freelist.h"
#include <limits.h>
#include <error.h>


void initFreeList(free_list *l1) {

    l1->size = FREE_LIST_SIZE;
    l1->rear = -1;
    for(int i=0;i<FREE_LIST_SIZE;i++) {

        l1->list[i].blockPtr = NULL;
        l1->list[i].blockSize = 0;
    }
    return;
}

void swapListElements(free_list *l1,int i,int j) {

    _free_list temp;
    temp = l1->list[j];
    l1->list[j] = l1->list[i];
    l1->list[i] = temp;
    return;
}

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

void heapifyFreeList(free_list *l1,int i) {


    if(l1->rear == 0)
        return;
    i=0;
    int max;
    while(i<= l1->rear) {

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
        i = max;
    }
    return;
}


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
    heapifyFreeList(l1,0);
    return 1;
}


int sortFreeList(free_list *l1) {

    if(!l1 || l1->list==NULL || l1->rear==-1)
        return INT_MIN;

    int i;
//TODO: chuklay , infinte loop madhe jael. Correct it
    for(i=0;i>=0;i++) {
        swapListElements(l1,0,l1->rear);
        heapifyFreeList(l1,0);
        l1->rear--;
    }

    return 1;
}


