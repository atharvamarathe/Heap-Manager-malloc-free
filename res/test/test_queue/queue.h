#include <stdlib.h>
#include <stdbool.h>
#include "malloc.h"

typedef struct data {
    char name[16];
    unsigned int age;
} data;

typedef struct node {
    data d;
    struct node *next;
} node;

typedef node* queue;

void qinit(queue *q);

bool qfull(queue *q);

bool qempty(queue *q);

void enq(queue *q, data d);

data deq(queue *q);
