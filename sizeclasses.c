#include <stdio.h>
#include <stdlib.h>
#include "sizeclasses.h"



page_list sizeClassList[NUM_OF_CLASSES][MAX_PAGES];


int classSizeArray[NUM_OF_CLASSES] = {

    4,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,160,176,184,192,208,224,240,256,272,288,312,336,368,408,448,480,512,576,640,704,768,896,1024
};

void initSizeClassList() {


    for(int j=0;j<NUM_OF_CLASSES-1;j++) {

        for(int i=0;i<MAX_PAGES;i++) {
            // printf("The pointer is : %p\n",*(sizeClassList[j][i].head));
            (sizeClassList[j][i].head) = (meta_data_block)getPages(1);
            // printf("Available bins : %ld\n",TOTAL_BINS_IN_CLASS(1,classSizeArray[j]));
            sizeClassList[j][i].availableBins = TOTAL_BINS_IN_CLASS(1,classSizeArray[j]);


        }
    }
    for(int i=0;i<MAX_PAGES;i++) {

        (sizeClassList[NUM_OF_CLASSES-1][i].head) = (meta_data_block)getPages(2);
        sizeClassList[NUM_OF_CLASSES-1][i].availableBins = TOTAL_BINS_IN_CLASS(2,1024);
    }
    // for(int i=0;i<NUM_OF_CLASSES;i++) {

    //     for(int j=1;j<MAX_PAGES;j++) {
    //             sizeClassList[i][j].head = NULL;
    //             sizeClassList[i][j].availableSize = -1;
    //     }
    // }

    // createSizeClassBinsList(sizeClassList[0][0].head,SIZECLASS4);
    for(int i=0;i<NUM_OF_CLASSES-1;i++) {

        for(int j=0;j<MAX_PAGES;j++) {
            if(sizeClassList[i][j].head != NULL) {
                // printf("NEW PAGE STARTING -----------------------------------------------------------------------------------------------------\n");
                createSizeClassBinsList(&(sizeClassList[i][j].head),classSizeArray[i],1);
            }
            // else 
                // perror("VM allocation failed \n");
        }
    }
    for(int j=0;j<MAX_PAGES;j++) {
        if(sizeClassList[NUM_OF_CLASSES-1][j].head != NULL) {
                // printf("NEW PAGE STARTING -----------------------------------------------------------------------------------------------------\n");
                createSizeClassBinsList(&(sizeClassList[NUM_OF_CLASSES-1][j].head),classSizeArray[NUM_OF_CLASSES-1],2);
        }

    }

}



void createSizeClassBinsList(meta_data_block *head,int binSize,int no_of_pages) {


    if(head == NULL) {
        printf("NULL detected\n");
        return;

    }

    // mmInit();
    int binNum;
    binNum = no_of_pages*SYSTEM_PAGE_SIZE/(binSize+METABLOCK_SIZE);


    // printf("SYSTEM_PAGE_SIZE IS %ld and biNum is : %d\n",SYSTEM_PAGE_SIZE,binNum);

    meta_data_block a,prev=NULL;
    a = *head;
    for(int i=0;i<binNum-1;i++) {
        // printf("AT prevBlock a: %p and prev is  %p\n",a,prev);
        a->prevBlock = prev;
        a->blockSize=binSize;
        // printf("Block Size : %d\n",a->blockSize);
        // printf("AT the is free \n");
        a->isFree = TRUE;
        // printf("At next Block \n");
        a->nextBlock =(meta_data_block)(((char *)a)+METABLOCK_SIZE+binSize);
        // printf("Binsize : %d , Current position : %p and nextBlock at %p\n",binSize,a,a->nextBlock);
        prev = a;
        a = a-> nextBlock;
    }
    
    // printf("PREV AFTER FOR LOOP IS %p\n",prev);
    a->prevBlock = prev;
    a->blockSize=binSize;
    // printf("Block Size : %d\n",a->blockSize);
    a->isFree = TRUE;
    // printf("/////////////////////////////////////////////////////////////////////////////\n");
    a->nextBlock = NULL;
}















