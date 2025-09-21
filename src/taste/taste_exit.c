#include "taste_exit.h"

#include "taste/test.h"

void taste_exit() {
	__asm__ volatile("li    a7, 0x8\n\t"
	                 "li    a6, 0\n\t"
	                 "ecall");
	__builtin_unreachable();
}
