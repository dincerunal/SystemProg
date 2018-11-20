/*----------------------------------------------------------------------
FILE        : endian.h
AUTHOR      : Oguz Karan
LAST UPDATE : 15/10/2018
PLATFORM    : Any



Copyleft (c) 1993 by C and System Programmers Association (CSD)
All Rights Free
-----------------------------------------------------------------------*/

#ifndef ENDIAN_H_
#define ENDIAN_H_

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif
int is_little_endian(void);
const char * endian(void);
uint16_t htob_uint16(uint16_t val);
uint16_t htol_uint16(uint16_t val);
uint16_t swap_uint16(uint16_t val);
int16_t swap_int16(int16_t val);
uint32_t swap_uint32(uint32_t val);
int32_t swap_int32(int32_t val);
int64_t swap_int64(int64_t val);
uint64_t swap_uint64(uint64_t val);

#ifdef __cplusplus 
}
#endif

#endif /* ENDIAN_H_ */
