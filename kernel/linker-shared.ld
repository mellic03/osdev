OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_dl_init)

SECTIONS
{
    . = 4K;
    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .text : {
        *(.text._dl_init )
        KEEP(*(.text*))
    }

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .rodata : {
        *(.rodata*)
    }

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    /* Discard .note.* and .eh_frame* since they may cause issues on some hosts. */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}