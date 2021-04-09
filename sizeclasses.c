#include <stdio.h>
#include <stdlib.h>
#include "sizeclasses.h"



page_list sizeClassList[NUM_OF_CLASSES][MAX_PAGES];


int classSizeArray[NUM_OF_CLASSES] = {

    4,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,160,176,184,192,208,224,240,256,272,288,312,336,368,408,448,480,512,576,640,704,768,896,1024
};

void initSizeClassList() {


    for(int i=0;i<NUM_OF_CLASSES;i++) {
        for(int j=0;j<MAX_PAGES;j++) {
            sizeClassList[i][j].head = NULL;
            sizeClassList[i][j].availableBins = -1;
        }
    }
    return;

}

void createSizeClassPage(int sizeclass,int offset) {

    if(sizeClassList[sizeclass][offset].head == NULL && sizeClassList[sizeclass][offset].availableBins == -1) {

         if(sizeclass != 41) {

        sizeClassList[sizeclass][offset].head = (meta_data_block)getPages(1);
        sizeClassList[sizeclass][offset].availableBins = TOTAL_BINS_IN_CLASS(1,classSizeArray[sizeclass]);
        createSizeClassBinsList(&(sizeClassList[sizeclass][offset].head),classSizeArray[sizeclass],1);
        }
        else if(sizeclass == 41) {
            sizeClassList[sizeclass][offset].head = (meta_data_block)getPages(2);
            sizeClassList[sizeclass][offset].availableBins = TOTAL_BINS_IN_CLASS(2,classSizeArray[sizeclass]);
            createSizeClassBinsList(&(sizeClassList[sizeclass][offset].head),classSizeArray[sizeclass],2);
        }
    }
    return ;
}


meta_data_block getPageforAllocation(int sizeclass) {
    int i=0;
    if(sizeClassList[sizeclass][0].head == NULL && sizeClassList[sizeclass][0].availableBins == -1) {
        createSizeClassPage(sizeclass,0);
        sizeClassList[sizeclass][0].availableBins -=1;
        return sizeClassList[sizeclass][0].head;
    }
    while(i < MAX_PAGES && sizeClassList[sizeclass][i].availableBins <1 && sizeClassList[sizeclass][i].availableBins != -1) {
        i++;
    }
    if(i >= MAX_PAGES) {
        return NULL;
    }
    else if(sizeClassList[sizeclass][i].availableBins >= 1) {
        sizeClassList[sizeclass][i].availableBins -=1;
        return sizeClassList[sizeclass][i].head;
    }
    else if(sizeClassList[sizeclass][i].availableBins == -1) {

        createSizeClassPage(sizeclass,i);
        sizeClassList[sizeclass][i].availableBins -=1;
        return sizeClassList[sizeclass][i].head;
    }
    return NULL;
}

void createSizeClassBinsList(meta_data_block *head,int binSize,int no_of_pages) {


    if(head == NULL) {
        return;

    }

    // mmInit();
    int binNum;
    binNum = no_of_pages*SYSTEM_PAGE_SIZE/(binSize+METABLOCK_SIZE);



    meta_data_block a,prev=NULL;
    a = *head;
    for(int i=0;i<binNum-1;i++) {
        a->prevBlock = prev;
        a->blockSize=binSize;
        a->isFree = TRUE;
        a->headPtr = *head;
        a->nextBlock =(meta_data_block)(((char *)a)+METABLOCK_SIZE+binSize);
        prev = a;
        a = a-> nextBlock;
    }
    
    a->prevBlock = prev;
    a->blockSize=binSize;
    a->isFree = TRUE;
    a->nextBlock = NULL;
}

int isSizeClassPageEmpty(int sizeclass, int offset) {

    meta_data_block mptr;

    mptr = sizeClassList[sizeclass][offset].head;
    while(mptr != NULL) {

        if(mptr ->isFree == FALSE)
            return FALSE;
        mptr = mptr -> nextBlock;
    }
    return TRUE;
}

void removeEmptySizeClassPage(int sizeclass, int offset) {
    //TODO : No need to recheck for isempty
    if(isSizeClassPageEmpty(sizeclass,offset) == TRUE) {

        int j=MAX_PAGES-1;
        while(j>=0 && sizeClassList[sizeclass][j].availableBins == -1 && sizeClassList[sizeclass][j].head == NULL)
            j--;
        if(j==0) {
            if(classSizeArray[sizeclass] == 1024)
                freePages(sizeClassList[sizeclass][j].head,2);
            else 
                freePages(sizeClassList[sizeclass][j].head,1);
            
            sizeClassList[sizeclass][j].head = NULL;
            sizeClassList[sizeclass][j].availableBins = -1;
            return;
        }
        page_list temp;
        temp = sizeClassList[sizeclass][offset];
        sizeClassList[sizeclass][offset] = sizeClassList[sizeclass][j];
        sizeClassList[sizeclass][j] = temp;
        //TODO : Handle case for 1024 size class
        if(classSizeArray[sizeclass] == 1024)
            freePages(sizeClassList[sizeclass][j].head,2);
        else
            freePages(sizeClassList[sizeclass][j].head,1);
        sizeClassList[sizeclass][j].head = NULL;
        sizeClassList[sizeclass][j].availableBins = -1;
    }
    return ;
}









