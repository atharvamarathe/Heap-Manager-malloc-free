#include <stdlib.h>
#include <stdio.h>
#include "malloc.h"

int main() { 
    int *a = (int*)myMalloc(512);
    int *b = (int*)myMalloc(16);
    int *c = (int*)myMalloc(134528);
    myFree(a);
    a = (int*)myMalloc(496);
    return 0;
}
//    for (int i = 464; i <= 528; i++)
//        printf("%d %d\n", i, PAD_REQUEST(i));
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
   
