#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"


int main(void)
{
    int i;
    
    for (i = 0; i <= 3307; ++i)
        if (is_port_available_v4(i))
            printf("%d\n", i);   
    
    
    return 0;    
}

