OUTPUT_FORMAT("binary")
phys = 0x0500;

SECTIONS
{
    .text phys : AT(phys)
    {
        code = .;
        *(.text)
        . = ALIGN(4096);
    }


    .data : AT(phys + (data - code))
    {
        data = .;
        *(.data)
        . = ALIGN(4096);
    }


    .bss : AT(phys + (bss - code))
    {
        bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    end = .;
}