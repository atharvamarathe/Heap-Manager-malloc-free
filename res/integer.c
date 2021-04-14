#include "integer.h"

void initInteger(Integer *a) {
    node *temp = (node*)myMalloc(sizeof(node));
    temp->digit = '0';
    temp->prev = NULL;
    temp->next = NULL;
    a->first = temp;
    a->last = temp;
}

void addDigit(Integer *a, char c) {
    node *temp = (node*)myMalloc(sizeof(node));
    temp->digit = c;
    temp->next = NULL;
    if (a->first->digit == '0') {
        temp->prev = NULL;
        a->first = temp;
    }
    else {
        temp->prev = a->last;
        a->last->next = temp;
    }
    a->last = temp;
}

void addDigitFront(Integer *a, char c) {
    static int added_zero = 0;
    if ((a->first->digit == '0') && !added_zero) {
        if (c == '0') {
            added_zero = 1;
        }
        a->last->digit = c;
    }
    else {
        added_zero = 0;
        if (c == '0') {
            added_zero = 1;
        }
        node *temp = (node*)myMalloc(sizeof(node));
        temp->digit = c;
        temp->prev = NULL;
        temp->next = a->first;
        a->first->prev = temp;
        a->first = temp;
    }
}

Integer createIntegerFromString(char *str) {
    Integer a;
    initInteger(&a);
    int i = 0;
    while (str[i] != '\0') {
        addDigit(&a, str[i]);
        i++;
    }
    return a;
}

void printInteger(Integer a) {
    node *ptr = a.first;
    while (ptr != NULL) {
        printf("%c", ptr->digit);
        ptr = ptr->next;
    }
}

void destroyInteger(Integer *a) {
    node *ptr = a->first, *temp;
    while (ptr != NULL) {
        temp = ptr;
        ptr = ptr->next;
        myFree(temp);
    }
    initInteger(a);
}

Integer addIntegers(Integer a, Integer b){
    int sum = 0, carry = 0;
    char e;
    Integer c;
    node *ptr1, *ptr2;
    
    initInteger(&c);
    ptr1 = a.last;
    ptr2 = b.last;

    while((ptr1 != NULL) && (ptr2 != NULL)){
        sum = ptr1->digit + ptr2->digit + carry - 2 * '0';
        if(sum > 9){
            carry = sum / 10;
            sum = sum % 10;
        }	
        else {
            carry = 0;
        }
        e = sum + '0';	
        addDigitFront(&c, e);
        ptr1 = ptr1->prev;
        ptr2 = ptr2->prev;

    }
    while(ptr1 != NULL){
        sum = (ptr1->digit - '0') + carry;
        e = sum + '0';
        addDigitFront(&c, e);
        ptr1 = ptr1->prev;
        carry = 0;
    }
    while(ptr2 != NULL){
        sum = (ptr2->digit - '0') + carry;
        e = sum + '0';
        addDigitFront(&c, e);
        ptr2 = ptr2->prev;
        carry = 0;
    }
    if(carry != 0) {
        e = carry + '0';
        addDigitFront(&c, e);
    }
    return c;
}

Integer substractIntegers(Integer a, Integer b) {
    int diff, x, y, borrow = 0;
    node *ptr1, *ptr2;
    Integer c;
    
    initInteger(&c);
    ptr1 = a.last;
    ptr2 = b.last;

    while ((ptr1 != NULL) && (ptr2 != NULL)) {
        x = ptr1->digit;
        y = ptr2->digit;
        x -= borrow;
        borrow = 0;
        if (x < y) {
            x += 10;
            borrow = 1;
        }
        diff = x - y + '0';
        addDigitFront(&c, (char)diff);
        ptr1 = ptr1->prev;
        ptr2 = ptr2->prev;
    }
    if ((ptr1 == NULL) && (ptr2 != NULL)) {
        destroyInteger(&c);
    }
    while (ptr1 != NULL) {
        x = ptr1->digit;
        x -= borrow;
        borrow = 0;
        addDigitFront(&c, (char)x);
        ptr1 = ptr1->prev;
    }
    if (borrow) {
        destroyInteger(&c);
    }
    return c;
}

