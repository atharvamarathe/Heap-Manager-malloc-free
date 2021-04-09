#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"
typedef struct buf {
    int a,b,c,d;
}buf;

typedef struct largealloc {

    char arr[1000];
}largealloc;

int main() {

    
    largealloc *buf;
    buf = (largealloc*)myMalloc(sizeof(largealloc));
    

    return 0;
 }




