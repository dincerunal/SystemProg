/*----------------------------------------------------------------------
FILE        : endian.c
AUTHOR      : Oguz Karan
LAST UPDATE : 14/02/2018
PLATFORM    : Any


Copyleft (c) 1993 by C and System Programmers Association (CSD)
All Rights Free
-----------------------------------------------------------------------*/

#include "endian.h"

int is_little_endian()
{
    int16_t a = 1;
    
    return *(char *)&a == 1;
}

const char *endian()
{
    return is_little_endian() ? "LITTLE ENDIAN" : "BIG ENDIAN";
}

uint16_t htob_uint16(uint16_t val)
{
    if (is_little_endian())
        val = swap_uint16(val);
    
    return val;
}

uint16_t htol_uint16(uint16_t val)
{
    if (!is_little_endian())
        val = swap_uint16(val);
    
    return val;
}

uint16_t swap_uint16(uint16_t val)
{
    return (val << 8) | (val >> 8 );
}

int16_t swap_int16(int16_t val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

uint32_t swap_uint32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

int32_t swap_int32(int32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

int64_t swap_int64(int64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

uint64_t swap_uint64(uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}
