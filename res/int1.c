#include "integer.h"

int main() {
    Integer a, b, sum, diff;

    initInteger(&a);
    initInteger(&b);

    char str2[200], ch;

    while((ch = getchar()) != '\n')
        addDigit(&a, ch);    
    
    scanf("%s", str2);
    
    b = createIntegerFromString(str2);
    printInteger(a);
    printf("\n");
    printInteger(b);
    printf("\n");
  
    sum = addIntegers(a, b);
    printInteger(sum);
    printf("\n");
    
    diff = substractIntegers(a, b);
    printInteger(diff);
    printf("\n");
}

