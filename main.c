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

    
    printf("Please enter the size of array you want to create :");
    int size,size1;
    scanf("%d",&size);

    char *arr;
    arr = (char *)myMalloc(size);
    arr[0] = 'a';
    arr[size-1] = 'b';
    printf("Done \n");
    char *em;
    printf("Enter size of another one :");
    scanf("%d",&size1);
    em = (char *)myMalloc(size1);
    em[0] = 'c';
    em[size1-1] = 'd';

    printf("First and last element of arr is %c and %c \n",arr[0],arr[size-1]);
    printf("First and last element of em is %c and %c \n",em[0],em[size1-1]);


    return 0;
 }




