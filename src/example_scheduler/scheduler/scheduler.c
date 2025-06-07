#include "scheduler.h"
#include <stdbigos/csr.h>

#define ENABLE_INTERRUPTS()  CSR_SET(mstatus, 8)
#define DISABLE_INTERRUPTS() CSR_CLEAR(mstatus, 8)

volatile process_t* volatile process_list[100];
volatile u32 process_count = 0;
volatile process_t* volatile current_process = NULL;


void mutex_lock(mutex_t* mutex) {
    u32 pid = current_process->pid;
    while (__sync_lock_test_and_set(&mutex->locked, 1)) {
    }
    mutex->owner_pid = pid;
}

void mutex_unlock(mutex_t* mutex) {
    u32 pid = current_process->pid;
    if (mutex->owner_pid == pid) {
        mutex->owner_pid = 0;
        __sync_lock_release(&mutex->locked);
    }
}

extern u8 bss_start;
extern u8 bss_end;
static const u64 clint_base = 0x02000000;
static volatile u64* mtime = (u64*)(clint_base + 0xBFF8);
static volatile u64* mtimecmp = (u64*)(clint_base + 0x4000);
static const u64 quant = 5000000llu;

void timer_interrupt_handler(void) {
    if (process_count > 0) {
        u32 idx = 0;
        if (current_process != NULL) {
            for (u32 i = 0; i < process_count; i++) {
                if (process_list[i] == current_process) {
                    idx = i;
                    break;
                }
            }
            idx = (idx + 1) % process_count;
            current_process = process_list[idx];
        }
        dprintf("Process %u\n", current_process->pid);
    } else {
        dputs("No processes to schedule\n");
    }

    mtimecmp[hartid()] = *mtime + quant;
}
 

[[gnu::interrupt("machine")]]
void handler() {
	reg_t cause = CSR_READ(mcause);
	if (is_interrupt(cause)) {
		reg_t int_no = get_interrupt_code(cause);

		switch (int_no) {
		case IntMTimer:
			timer_interrupt_handler();
			break;
		default: dprintf("unknown interrupt (%ld)\n\t", int_no); break;
		}

		CSR_CLEAR(mip, (reg_t)1 << int_no);
		return;
	}
    else {
        dprintf("Exception %ld 0x%x\n", cause, CSR_READ(mepc));
    }
}

void timer_init(void) {
	memset(&bss_start, '\0', &bss_end - &bss_start);

	// register handler
	CSR_WRITE(mtvec, handler);

	// request a timer interrupt
	mtimecmp[hartid()] = *mtime + quant;

	// set MIE in mstatus
	CSR_SET(mstatus, 8);

	// set TIMER in mie
	CSR_SET(mie, 1lu << IntMTimer);
    return;
}

void scheduler_init(void) {
    for(int i = 0; i < 100; ++i) {
        process_list[i] = NULL;
    }
    
    timer_init();
    dputs("Timer set\n");
}


void scheduler_add_process(process_t* process) {
    if (process_count < 100 && process != NULL) {
        process_list[process_count++] = process;
        if (current_process == NULL) {
            current_process = process;
        }
        // dprintf("Process added, process count: (%ld)\n", process_count);
        // dprintf("process list %u\n", process_list);
        // for(int i = 0; i < process_count; i++) {
        //     dprintf("Process %u\n", process_list[i]);
        // }
    }
}

void scheduler_remove_process(u32 process_id) {
    return;
}