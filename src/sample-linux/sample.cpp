#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#define SIZE    100000

using namespace std;

void do_parallel()
{
    int i;
    int a[SIZE], b[SIZE];
    vector<int> vec(SIZE);
    
    for (i = 0; i < SIZE; ++i)
        a[i] = rand() % 100;
    
    for (i = 0; i < SIZE; ++i)
        b[i] = rand() % 100;
    
    #pragma omp parallel for
        for (i = 0; i < SIZE; ++i)        
            vec.push_bac
        
    cout << "size=" << vec.size() << endl;    
}

int main()
{
    do_parallel();  

    return 0;
}


