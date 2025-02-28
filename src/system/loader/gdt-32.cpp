#include "./gdt-32.hpp"
#include <stdc++/cstring.hpp>



// void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other, gdtdesc *desc)
// {
// 	desc->lim0_15 = (limite & 0xffff);
// 	desc->base0_15 = (base & 0xffff);
// 	desc->base16_23 = (base & 0xff0000) >> 16;
// 	desc->acces = acces;
// 	desc->lim16_19 = (limite & 0xf0000) >> 16;
// 	desc->other = (other & 0xf);
// 	desc->base24_31 = (base & 0xff000000) >> 24;
// 	return;
// }

extern "C" {
    void _nasm_gdt_load( ck_gdt_table*, ck_TSS* );
}


void ckGDT32_create( ck_gdt_table *GDT, ck_TSS *tss )
{
    for (int i=0; i<6; i++)
    {
        GDT->entries[i] = {
            0, 0xFFFFF, 0, 0xC
        };
    }

    GDT->null_desc = {0, 0, 0, 0};

    GDT->kernel_mode_code.access = 0x9A;
    GDT->kernel_mode_data.access = 0x92;

    GDT->user_mode_code.access = 0xFA;
    GDT->user_mode_data.access = 0xF2;
    
    GDT->task_state = {
        .base   = (uint32_t)(tss),
        .limit  = sizeof(ck_TSS)-1,
        .access = 0x89,
        .flags  = 0x0
    };

}


void ckGDT32_load( ck_gdt_table *GDT, ck_TSS *tss )
{
    _nasm_gdt_load(GDT, tss);
}


