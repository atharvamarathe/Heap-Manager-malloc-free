#include <stdio.h>
#include <stdlib.h>
// #include "mm.h"
#include "sizeclasses.h"
#include <sys/mman.h>
#include <unistd.h>
#include <memory.h>
typedef struct buf {
    int a,b,c,d;
}buf;

typedef struct largealloc {

    char arr[1000];
}largealloc;

int main() {

    // mm_init();
//    printf("VM Page Size = %lu\n",SYSTEM_PAGE_SIZE);
//    void *addr1 = getPages(1);
//    void *addr2 = getPages(1);
//    printf("page 1 = %p, page 2 = %p\n",addr1,addr2);

//    mm_init(&d1);
//    printf("Verifying that block is free %d \n",d1 ->metaData.isFree);
    // buf *a;
    // a= (buf*)Malloc(sizeof(buf));
    // if(a == NULL)
    //         printf("Sorry amlloc didnt work\n");
    // a->a = 1;
    // a->b =2;
    // a->c=3;
    // a->d=4;
    // printf("The number is %d\n",a->d);
    buf *d;
    d=(buf*)myMalloc(sizeof(buf));
    d->a= 24;
    d->b=567;
    d->c=444;
    d->d=2222;
    printf("the value of the struct is %d %d %d and %d\n",d->a,d->b,d->c,d->d);
    myFree(d);
    buf *try,*b,*c;
    try = (buf*)myMalloc(sizeof(buf));
    try->a= 3024;
    try->b=3024;
    try->c=3024;
    try->d=3024;
    printf("the value of the struct is %d %d %d and %d\n",try->a,try->b,try->c,try->d);
    largealloc *ptr1,*ptr2,*ptr3,*ptr4,*ptr5,*ptr6,*ptr7,*ptr8,*ptr9,*ptr10;
    ptr1 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr2 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr3 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr1->arr[0] = 1;
    ptr2->arr[0] = 2;
    ptr3->arr[0] = 3;
    printf("ptr1 : %d\n",ptr1->arr[0]);
    printf("ptr2 : %d\n",ptr2->arr[0]);
    printf("ptr3 : %d\n",ptr3->arr[0]);
    myFree(ptr1);
    myFree(ptr2);
    myFree(ptr3);
    ptr4 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr5 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr6 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr4->arr[0] = 4;
    ptr5->arr[0] = 5;
    ptr6->arr[0] = 6;
    printf("ptr4 : %d\n",ptr4->arr[0]);
    printf("ptr5 : %d\n",ptr5->arr[0]);
    printf("ptr6 : %d\n",ptr6->arr[0]);
    myFree(ptr4);
    myFree(ptr5);
    myFree(ptr6);
    ptr7 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr8 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr9 = (largealloc *)myMalloc(sizeof(largealloc));
    ptr7->arr[0] = 7;
    ptr8->arr[0] = 8;
    ptr9->arr[0] = 9;
    printf("ptr7 : %d\n",ptr7->arr[0]);
    printf("ptr8 : %d\n",ptr8->arr[0]);
    printf("ptr9 : %d\n",ptr9->arr[0]);
    myFree(ptr7);
    myFree(ptr8);
    myFree(ptr9);
    ptr10 = (largealloc *)myMalloc(sizeof(largealloc));


    
    
    
    ptr10->arr[0] = 10;
    // printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Printing the results :&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
    
    
   
    printf("ptr10 : %d\n",ptr10->arr[0]);

    // ptr1 = (largealloc *)myMalloc(sizeof(largealloc));

    // b = (buf*)myMalloc(sizeof(buf));
    // c = (buf*)myMalloc(sizeof(buf));
    // largealloc *p;
    // p = (largealloc *)myMalloc(sizeof(largealloc));
    // printf("The value of class 16 is : %d\n",GETCLASSINDEX(16));
    // Free(d);
    // Free(a);
    return 0;
 }




