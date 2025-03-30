OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
    . = 0xBEBE0000;
    .text : {
        *(.text .text.*)
        *(._start ._start.*)
    } :text

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    .data : {
        *(.data .data.*)
    } :data

    /* . = ALIGN(16);
    .init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } :data */

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

}
