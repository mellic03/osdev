OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_dl_init)

SECTIONS
{
    . = 0x1000;
    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .text : {
        *(.text._dl_init )
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

    .bss : {
        *(.bss*)
        *(COMMON)
    }

}