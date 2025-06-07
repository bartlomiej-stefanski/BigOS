#ifndef _KERNEL_SCHEDULER_H_
#define _KERNEL_SCHEDULER_H_

#include <stdbigos/sbi.h>
#include <debug/debug_stdio.h>
#include <stdbigos/csr.h>
#include <stdbigos/string.h>
#include <stdbigos/trap.h>
#include <stdbigos/types.h>


typedef struct {
    volatile u32 locked;
    u32 owner_pid;
} mutex_t;

void mutex_lock(mutex_t* mutex);
void mutex_unlock(mutex_t* mutex);

typedef struct process {
    u32 pid;
    void* stack_ptr;
} process_t;

void scheduler_init(void);
void scheduler_add_process(process_t* process);
void scheduler_remove_process(u32 pid);
void timer_init(void);
#endif