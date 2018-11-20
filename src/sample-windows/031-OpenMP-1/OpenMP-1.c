#include <stdio.h>
#include <Windows.h>
#include <omp.h>

int main(void)
{
	long long int i;
	LARGE_INTEGER li1, li2, freq;
	__int64 result;

	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&li1);

#pragma omp parallel for 
	for (i = 0; i < 1000000000LL; ++i) {
		/* ... */
	}

	QueryPerformanceCounter(&li2);

	result = li2.QuadPart - li1.QuadPart;
	printf("%f\n", (double)result / freq.QuadPart);

	QueryPerformanceCounter(&li1);


	for (i = 0; i < 1000000000LL; ++i) {
		/* ... */
	}

	QueryPerformanceCounter(&li2);

	result = li2.QuadPart - li1.QuadPart;
	printf("%f\n", (double)result / freq.QuadPart);

	return 0;
}
