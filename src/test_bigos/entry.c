#include <stdbigos/sbi.h>
#include <stdbigos/types.h>
#include <taste/taste.h>
#include <taste/test.h>

void main([[maybe_unused]] u32 hartid, [[maybe_unused]] const void* fdt) {
	INCLUDE_SUITE(sanity_check)
	INCLUDE_SUITE(vfs)
	run_tests();
}
