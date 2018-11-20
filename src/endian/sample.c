#include <stdio.h>
#include "endian.h"

int main(void)
{
    uint16_t val = 0xA;
    
    printf("%X\n", htob_uint16(val));
    printf("%X\n", htol_uint16(val));
    
    
    return 0;
}

