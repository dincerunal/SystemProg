#include <stdio.h>
#include <omp.h>

int main(void)
{
    int i;
    int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int b[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int c[10];


    #pragma omp simd
        for (i = 0; i < 10; ++i)
            c[i] = a[i] * b[i];

#pragma omp parallel
        {
            printf("%d\n", omp_get_thread_num());

        }

    for (i = 0; i < 10; ++i)
        printf("%d ", c[i]);
    printf("\n");

    return 0;
}
