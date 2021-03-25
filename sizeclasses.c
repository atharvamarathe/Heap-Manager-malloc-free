#include <stdio.h>
#include <stdlib.h>
#include "sizeclasses.h"



int classSizeArray[NUM_OF_CLASSES] = {

    4,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,160,176,184,192,208,224,240,256,272,288,312,336,368,408,448,480,512,576,640,704,768,896,1024
};

void initSizeClassList() {


    for(int j=0;j<NUM_OF_CLASSES-1;j++) {

        for(int i=0;i<MAX_PAGES;i++) {

            *(sizeClassList[j][i].head) = (meta_data_block)getPages(1);
            sizeClassList[j][i].availableBins = TOTAL_BINS_IN_CLASS(1,classSizeArray[j]);


        }
    }
    for(int i=0;i<MAX_PAGES;i++) {

        *(sizeClassList[NUM_OF_CLASSES-1][i].head) = (meta_data_block)getPages(2);
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

            createSizeClassBinsList(sizeClassList[i][j].head,classSizeArray[i],1);

        }
    }
    for(int j=0;j<MAX_PAGES;j++) {

    createSizeClassBinsList(sizeClassList[NUM_OF_CLASSES-1][j].head,classSizeArray[NUM_OF_CLASSES-1],2);

    }

}



void createSizeClassBinsList(meta_data_block *head,int binSize,int no_of_pages) {


    if(head == NULL)
        return;

    // mmInit();
    int binNum;
    binNum = no_of_pages*SYSTEM_PAGE_SIZE/(binSize+METABLOCK_SIZE);
    meta_data_block a,prev=NULL;
    a = *head;
    for(int i=0;i<binSize-1;i++) {
        a->prevBlock = prev;
        a->blockSize=binSize;
        a->isFree = TRUE;
        a->nextBlock =(meta_data_block)(((char *)a)+METABLOCK_SIZE+binSize);
        prev = a;
        a = a-> nextBlock;
    }
    //last block
    a->prevBlock = prev;
    a->blockSize=binSize;
    a->isFree = TRUE;
    a->nextBlock = NULL;
}















