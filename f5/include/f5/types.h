#ifndef _F5_TYPES_H_
#define _F5_TYPES_H_

#include <stddef.h>
#include <f5/stdint.h>

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
#ifndef _WIN64
typedef signed long s32;
typedef unsigned long u32;
#else
typedef int32_t s32;
typedef uint32_t u32;
#endif

typedef int64_t s64;
typedef uint64_t u64;

struct v32 {
  float x;
  float y; 
  float z;
};

#ifndef SBIG
#define SBIG(q) (((q)&0x000000FF) << 24 | ((q)&0x0000FF00) << 8 | ((q)&0x00FF0000) >> 8 | ((q)&0xFF000000) >> 24)
#endif

#endif // _F5_TYPES_H_