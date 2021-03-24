#include <stdio.h>
#include <stdlib.h>
#include "mm.h"


typedef struct buf {
    int a,b,c,d;
}buf;

int main() {

    // mm_init();

//    printf("VM Page Size = %lu\n",SYSTEM_PAGE_SIZE);
//    void *addr1 = getPages(1);
//    void *addr2 = getPages(1);
//    printf("page 1 = %p, page 2 = %p\n",addr1,addr2);

//    mm_init(&d1);
//    printf("Verifying that block is free %d \n",d1 ->metaData.isFree);
    buf *a;
    a= (buf*)Malloc(sizeof(buf));
    if(a == NULL)
            printf("Sorry amlloc didnt work\n");
    a->a = 1;
    a->b =2;
    a->c=3;
    a->d=4;
    printf("The number is %d\n",a->d);
    buf *d;
    d=(buf*)Malloc(sizeof(buf));
    d->a= 24;
    d->b=567;
    d->c=444;
    d->d=2222;
    printf("the value of the struct is %d %d %d and %d\n",d->a,d->b,d->c,d->d);
    printf("The value of class 16 is : %d\n",GETCLASSINDEX(16));
    Free(d);
    Free(a);
    
    return 0;
 }




