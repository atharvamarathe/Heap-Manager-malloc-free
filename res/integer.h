#include <stdlib.h>
#include <stdio.h>
#include "malloc.h"

typedef struct node {
    char digit;
    struct node* prev;
    struct node* next;
} node;

typedef struct Integer {
    node *first;
    node *last;
} Integer;

void initInteger(Integer *a);

void addDigitFront(Integer *a, char c);

void addDigit(Integer *a, char c); 

Integer createIntegerFromString(char *str);

void printInteger(Integer a);

void destroyInteger(Integer *a);

Integer addIntegers(Integer a, Integer b);

Integer substractIntegers(Integer a, Integer b);

    
