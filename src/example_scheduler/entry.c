#include <debug/debug_stdio.h>
#include <stdbigos/csr.h>
#include <stdbigos/string.h>
#include <stdbigos/trap.h>
#include <stdbigos/types.h>
#include <scheduler/scheduler.h>


void main() {
	scheduler_init();
	volatile process_t p1 = {1, NULL};
	volatile process_t p2 = {2, NULL};
	volatile process_t p3 = {3, NULL};
	scheduler_add_process(&p1);
	scheduler_add_process(&p2);
	scheduler_add_process(&p3);
	while(1) {
		dputs("Main, robimy cos innego\n");
		for (volatile u64 i = 0; i < 999999; ++i) {
			for (volatile u64 j = 0; j < 999; ++j) {
				// Busy wait :)
			}
		}
	};
}


[[noreturn]]
void start() {
	main();
	while (true) wfi();
}

[[gnu::section(".init"), gnu::naked]]
void _start() {
	__asm__(".option push\n\t"
	        ".option norelax\n\t"
	        "la    gp, global_pointer\n\t"
	        ".option pop\n\t"
	        "la    sp, stack_top\n\t"
	        "j start");
}
