#include <stdlib.h>
#include <stdio.h>
#include "malloc.h"
#include "bins.h"

int main() { 
    int *a = (int*)myMalloc(28);
    int *b = (int*)myMalloc(45);
    myFree(a);
    a = (int*)myMalloc(24);
    return 0;
}
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
   
