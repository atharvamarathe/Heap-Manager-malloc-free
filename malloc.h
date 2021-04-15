#ifndef MALLOC_H
#define MALLOC_H


#include <stdint.h>
#include <stddef.h>

void*  myMalloc(size_t bytes);

void*  myCalloc(size_t num,size_t size);

void* myRealloc(void* ptr, size_t size);


void  myFree(void *ptr);


#endif