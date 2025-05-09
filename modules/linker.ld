OUTPUT_FORMAT(elf64-x86-64)
/* OUTPUT_FORMAT(binary) */

ENTRY(init)

SECTIONS
{
    . = 0x1000;
    . = ALIGN(CONSTANT(MAXPAGESIZE));
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

    .bss : {
        *(.bss*)
        *(COMMON)
    }

}
