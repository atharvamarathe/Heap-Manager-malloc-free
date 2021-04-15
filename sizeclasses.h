#ifndef SIZECLASSES_H
#define SIZECLASSES_H

#include "memoryallocator.h"
#include <stddef.h>
#define STRINGIFY(a,b) a ## b
#define GETCLASSINDEX(a) STRINGIFY(CLASS_,a)
#define MAX_PAGES 5
#define CLASS_SIZE_LIMIT 1024
#define NUM_OF_CLASSES 42
//  ________________________________________________________________________________
//  |  | CLASS-NAME  |   INDEX_OF_PAGE_LIST  | Max no of Pages Allocation capacity |  
//  -------------------------------------------------------------------------------|
#define CLASS_4                 0           //                 5                   |           
#define CLASS_8                 1           //                 5                   |                          
#define CLASS_16                2           //                 5                   |
#define CLASS_24                3           //                 5                   |
#define CLASS_32                4           //                 5                   |
#define CLASS_40                5           //                 5                   |
#define CLASS_48                6           //                 5                   |
#define CLASS_56                7           //                 5                   |
#define CLASS_64                8           //                 5                   |
#define CLASS_72                9           //                 5                   |
#define CLASS_80                10          //                 5                   |
#define CLASS_88                11          //                 5                   |
#define CLASS_96                12          //                 5                   |
#define CLASS_104               13          //                 5                   |
#define CLASS_112               14          //                 5                   |
#define CLASS_120               15          //                 5                   |
#define CLASS_128               16          //                 5                   |
#define CLASS_136               17          //                 5                   |
#define CLASS_144               18          //                 5                   |
#define CLASS_160               19          //                 5                   |
#define CLASS_176               20          //                 5                   |
#define CLASS_184               21          //                 5                   |
#define CLASS_192               22          //                 5                   |
#define CLASS_208               23          //                 5                   |
#define CLASS_224               24          //                 5                   |
#define CLASS_240               25          //                 5                   |
#define CLASS_256               26          //                 5                   |
#define CLASS_272               27          //                 5                   |
#define CLASS_288               28          //                 5                   |
#define CLASS_312               29          //                 5                   |
#define CLASS_336               30          //                 5                   |
#define CLASS_368               31          //                 5                   |
#define CLASS_408               32          //                 5                   |
#define CLASS_448               33          //                 5                   |
#define CLASS_480               34          //                 5                   |
#define CLASS_512               35          //                 5                   |
#define CLASS_576               36          //                 5                   |
#define CLASS_640               37          //                 5                   |
#define CLASS_704               38          //                 5                   |
#define CLASS_768               39          //                 5                   |
#define CLASS_896               40          //                 5                   |
#define CLASS_1024              41          //                 10                  |
//---------------------------------------------------------------------------------|

#define TOTAL_BINS_IN_CLASS(no_of_pages,class_size)  (no_of_pages*SYSTEM_PAGE_SIZE)/(METABLOCK_SIZE+class_size)

/*
 * page_list : Structure for the sizeclasses implementation. The sizeclassList is a 2D array of page_list
 * Members :
 *          head : pointer to the virtual memory page
 *          availableBins : Number of memory blocks available for use
 */

typedef struct page_list {

    meta_data_block head;
    int availableBins;
}page_list;


void initSizeClassList();

void createSizeClassBinsList(meta_data_block *head,int binSize,int no_of_pages);

void createSizeClassPage(int sizeclass,int offset);

meta_data_block getPageforAllocation(int sizeclass);

int isSizeClassPageEmpty(int sizeclass, int offset);

void removeEmptySizeClassPage(int sizeclass, int offset);

extern int classSizeArray[NUM_OF_CLASSES];

static page_list pageList[MAX_PAGES];

extern page_list sizeClassList[NUM_OF_CLASSES][MAX_PAGES];

#endif