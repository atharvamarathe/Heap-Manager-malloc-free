#include "circ_linked_list.h"

/* Gives the length of given linked list */
int length(list *l) {
    int i = 1;
    node *ptr = *l;
    if (*l == NULL) {
        return 0;
    }
    while (ptr->next != *l) {
        ptr = ptr->next;
        i++;
    }
    return i;
}

/* Initializes the list to NULL */
void init(list *l) {
    *l = NULL;
}

/* Inserts data at desired position in list 
 * Handles following cases:
 * 1. Invalid position 
 * 2. Insert at begining 
 * 3. Insert at any other position 
 * */
void insert(list *l, char *data, int pos) {
    node *temp;
    temp = (node*)myMalloc(sizeof(node));
    strcpy(temp->str, data);

    if ((pos > length(l)+1) || (pos < 1)) {
        printf("Did nothing\n");
        return;
    }
    else if ((*l == NULL) || (pos == 1)) {
        temp->next = temp;
        *l = temp;
    }
    else {
        int i;
        node *ptr = *l;
        for (i = 0; i < pos-2; i++) {
            ptr = ptr->next;
        }
        temp->next = ptr->next;
        ptr->next = temp;
    }
}

/* Stores all the nodes' data in a static string and returns it */
char* traverse(list *l) {
    static char str[200];
    strcpy(str, "\0");
    node *ptr = *l;
    do {
        strcat(str, ptr->str);
        strcat(str, " ");
        ptr = ptr->next;
    }
    while (ptr != *l);
    return str;
}

/* Removes duplicate elements in a list */
void remDuplicate(list *l) {
    node *ptr1 = *l, *ptr2, *temp;
    do {
        ptr2 = ptr1;
        do {
            if (!strcmp(ptr1->str, ptr2->next->str) && (ptr1 != ptr2->next)) {
                temp = ptr2->next;
                ptr2->next = ptr2->next->next;
                myFree(temp);
            }
            ptr2 = ptr2->next;
        }
        while (ptr2 != *l);
        ptr1 = ptr1->next;
    }
    while (ptr1 != *l);
}

/* Prints intersection of two lists */
void intersectlist(list *l1, list *l2) {
    remDuplicate(l1);
    remDuplicate(l2);
    node *ptr1 = *l1, *ptr2;
    char str[50] = "\0";
    do {
        ptr2 = *l2;
        do {
            if (!strcmp(ptr1->str, ptr2->str)) {
                strcat(str, ptr1->str);
                strcat(str, " ");
            }
            ptr2 = ptr2->next;
        }
        while (ptr2 != *l2);
        ptr1 = ptr1->next;
    }
    while (ptr1 != *l1);
    //printf("[%s]\n", str);
}


int main() { 
    list l, l2;
    char str[100];

    /* Initialization */
    init(&l);
    init(&l2);

    /* Insertion */
    insert(&l, "coll", 1);
    insert(&l, "ege", 2);
    insert(&l, "pune", 3);
    insert(&l, "ege", 4);
    insert(&l2, "ege", 1);
    insert(&l2, "coll", 2);

    /* Display */
    strcpy(str, traverse(&l));
    printf("[%s]\n", str);
    strcpy(str, traverse(&l2));
    printf("[%s]\n", str);

    /* Print intersection by first removing duplicate elements */
    intersectlist(&l, &l2);
    return 0;
}
