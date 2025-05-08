OUTPUT_FORMAT(elf64-x86-64)

ENTRY(init)

SECTIONS
{
    . = 0xA000;
    .text : {
        *(.text.init )
        *(.text*)
    }

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .rodata : {
        *(.rodata*)
    }

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .data : {
        *(.data*)
    }

    /* . = ALIGN(CONSTANT(MAXPAGESIZE));
    .init_array : {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);
    } :data */

    .bss : {
        *(.bss*)
        *(COMMON)
    }

}
