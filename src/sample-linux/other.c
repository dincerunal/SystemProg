/*----------------------------------------------------------------------------------------------------------------------
    Aşağıdaki ilkdeğer verme sentaksı C99 ile birlikte C ye eklenmiştir. Ancak
    gcc de çok önceden beri kullanılabilmektedir
----------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>

typedef struct tagDATE {
    int day, mon, year;    
} DATE;

int main(void)
{
    DATE d = {
        .day = 10,
        .mon = 9,
        .year = 1976
    };    
    
    printf("%02d.%02d.%d\n", d.day, d.mon, d.year);
    
    return 0;
}
