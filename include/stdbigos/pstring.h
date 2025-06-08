#ifndef STDBIGOS_PSTRING_H
#define STDBIGOS_PSTRING_H

#include <stdbigos/types.h>

/// Pascal-like length based string
typedef struct {
	u32 len;
	char* data;
} pstring_t;

/// Wrap null terminated string in `pstring_t` stuct
pstring_t pstring_l2w(char* str);

/// Returns the length of the string.
/// If ps is `NULL` or does not have `data` then `0` will be returned
u32 pstring_len_silenterr(const pstring_t* ps);

size_t pstring_memcpy(pstring_t* dest, const pstring_t* src);

/// Fills the `ps` with `val` value
/// Depends on memory being already allocated
u32 pstring_fill(pstring_t* ps, char val);

/// Copy `count` bytes from `src` to `dest`. Bounds are checked.
/// Returns the number of bytes copied.
size_t pstring_memmove(pstring_t* dest, const pstring_t* src, size_t count);

int pstring_strcmp(const pstring_t* lhs, const pstring_t* rhs);

char* pstring_strchr(const pstring_t* ps, char ch);

/// Returns a slice view for a given string.
/// Characters of slice view represent range `[start, end)`
pstring_t pstring_slice_view(const pstring_t* ps, u32 start, u32 end);

#endif
