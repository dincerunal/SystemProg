#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main()
{
	vector<int> vect;
	
	#pragma omp parallel  shared(vect)
	{
		int i = 0;

		for (i = 0; i < 10000; ++i)
			#pragma omp critical
			vect.push_back(i);
	}

	cout << vect.size() << endl;

	return 0;
}

