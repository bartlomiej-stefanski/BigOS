#ifndef _STDBIGOS_TYPES_H_
#define _STDBIGOS_TYPES_H_

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef unsigned long reg_t;
typedef signed long ireg_t;
static_assert(sizeof(reg_t) * 8 == __riscv_xlen);

typedef enum {
	ENDIAN_LITTLE = 0,
	ENDIAN_BIG = 1,
} endianness_t;

/// pstring_t:
/// either (P)ascal-like length based string
/// or (p)edantically checked string
typedef struct {
	size_t len;
	u8* data;
} pstring_t;

#include <stdbigos/meta/err_x_t.h>

ERRX_WRAPPER_T(pstring, pstring_t);
ERRX_WRAPPER_T(size, size_t);
ERRX_WRAPPER_T(int, int);

#endif
