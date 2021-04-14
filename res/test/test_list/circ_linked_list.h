#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

typedef struct node {
	char str[16];
	struct node *next;
} node;

typedef node* list;

int length(list *l);

void init(list *l);

void insert(list *l, char *data, int pos);

char* traverse(list *l);

void remDuplicate(list *l);

void intersectlist(list *l1, list *l2);
