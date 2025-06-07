ENTRY(_start);

MEMORY {
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128K
}

SECTIONS {
    .text : ALIGN(4K) {
        *(.init);
        *(.text);
    } > RAM

    .rodata : ALIGN(4K) {
        *(.rodata);
    } > RAM

    .data : ALIGN(4K) {
		*(.data.process_list)
        *(.data);
    } > RAM

    .bss : ALIGN(4K) {
        PROVIDE(bss_start = .);
        *(.bss);
        *(COMMON);
        PROVIDE(bss_end = .);
		PROVIDE(global_pointer = .);
    } > RAM

    /* STACK SECTION - SEPARATE FROM BSS */
    .stack (NOLOAD) : ALIGN(4K) {
        . += 4K;                /* Reserve 4KB stack */
        PROVIDE(stack_top = .); /* Stack grows downward */
    } > RAM
}

