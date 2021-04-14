#include <stdlib.h>
#include <stdio.h>
#include "malloc.h"

int main() { 
    int *a = (int*)myMalloc(1024);
    for (int i = 0; i < 256; i++) {
        *(a+i) = i;
        printf("%d\n", *(a+i));
    }
    return 0;
}
//    printf("\n");
//    print(a, (a+300));
//    chunk c1 = {640, 480};
//    chunk c2 = {480, 496};
//    chunk c3 = {128, 304};
//    chunk c4 = {128, 304};
//    chunk c5 = {128, 304};
//    
//    insert_small_chunk(g_mstate, &c1, 480);
//    insert_small_chunk(g_mstate, &c2, 496);
//    insert_small_chunk(g_mstate, &c3, 304);
//    insert_small_chunk(g_mstate, &c4, 304);
//    insert_small_chunk(g_mstate, &c5, 304);
// 135104   
