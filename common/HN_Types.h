#ifndef __HN_TYPES_H__
#define __HN_TYPES_H__

#include <sys/types.h>

#ifndef NO_U_INT_T
typedef u_int8_t uint8;
typedef u_int16_t uint16;
typedef u_int32_t uint32;
#else
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
#endif

typedef int8_t sint8;
typedef int16_t sint16;
typedef int32_t sint32;

#ifndef NULL
#define NULL (0L)
#endif

#endif
