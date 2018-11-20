#include <stdio.h>
#include "mylib.h"

void display_add(int a, int b)
{
    printf("%d + %d = %d\n", a, b, add(a, b));         
}

void display_mul(int a, int b)
{
    printf("%d * %d = %d\n", a, b, mul(a, b));
         
}