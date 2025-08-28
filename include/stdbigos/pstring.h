#ifndef STDBIGOS_PSTRING_H
#define STDBIGOS_PSTRING_H

#include <stdbigos/types.h>

// Some of the following functions that are string.h lookalikes are
// more-or-less analogous to their [xxx]n[yyy] counterparts but check
// correctness of the pstring_t argument.

/// Wrap null terminated string in `pstring_t` stuct
[[nodiscard]]
pstring_or_err_t pstring_l2w(char* str);

/// Returns the length of the pstring_t.
[[nodiscard]]
size_or_err_t pstring_len(const pstring_t* ps);

/// Returns the length of the pstring_t.
/// If ps is `NULL` or does not have `data` then `0` will be returned
[[nodiscard]]
size_t pstring_len_silenterr(const pstring_t* ps);

[[nodiscard]]
size_or_err_t pstring_memcpy(pstring_t* dest, const pstring_t* src);

/// Fills the `ps` with `val` value
/// Depends on memory being already allocated
[[nodiscard]]
error_t pstring_fill(pstring_t* ps, u8 val);

/// Copy `count` bytes from `src` to `dest`. Bounds are checked.
/// Returns the number of bytes copied.
[[nodiscard]]
size_or_err_t pstring_memmove(pstring_t* dest, const pstring_t* src, size_t count);

[[nodiscard]]
int_or_err_t pstring_strcmp(const pstring_t* lhs, const pstring_t* rhs);

[[nodiscard]]
pstring_or_err_t pstring_strchr(const pstring_t* ps, u8 ch);

/// Returns a slice view for a given string.
/// Characters of slice view represent range `[start, end)`
[[nodiscard]]
pstring_or_err_t pstring_slice_view(const pstring_t* ps, u32 start, u32 end);

/// Writes bytes from `src2` into `src1` starting at position `from` in `src1`.
/// Returns the number of bytes from `src2` written to `src1`.
[[nodiscard]]
size_or_err_t pstring_cat(pstring_t* src1, size_t from, const pstring_t* src2);

/// Concatenates two pstrings into `dest`.
/// Returns the number of bytes written into dest.
[[nodiscard]]
size_or_err_t pstring_concat(pstring_t* dest, const pstring_t* first, const pstring_t* second);

#endif
