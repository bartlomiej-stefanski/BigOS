#ifndef _MOCKS_LONGJUMP_H
#define _MOCKS_LONGJUMP_H

#include <stdbigos/types.h>

typedef struct JumpBuff_t {
	u64 sp;
	u64 s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
	u64 return_address;
} JumpBuff_t;

/// Save current environment to jump_buff.
/// Returns `0` after environment save and other values when jumping back to environment.
[[nodiscard]] u64 set_long_jump(JumpBuff_t* jump_buff);

/// Jump back to saved environment and return `val`.
[[noreturn]] void long_jump(JumpBuff_t* jump_buff, u64 val);

#endif
