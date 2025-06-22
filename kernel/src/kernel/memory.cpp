#include <kernel/memory.hpp>
#include <util/bitmanip.hpp>
#include <kpanic.hpp>
#include <stdint.h>

// #define BUMP_SIZE (16*idk::MEGA)
static constexpr size_t BUMP_SIZE = 16*idk::MEGA;
static uint8_t bump_buf[BUMP_SIZE];
static uintptr_t bump_base;
static uintptr_t bump_top;

void kmem::init()
{
    bump_base = (uintptr_t)bump_buf;
    bump_top  = idk::align_up(bump_base, 16);
}

void *kmem::bumpAlloc( size_t nbytes )
{
    uintptr_t addr = idk::align_up(bump_top, 16);
    bump_top += nbytes;

    if ((bump_top-bump_base) >= BUMP_SIZE)
    {
        kpanic(
            "[kmem::bumpAlloc] out of memory (%luB / %luB)",
            (bump_top-bump_base), BUMP_SIZE
        );
    }

    return (void*)addr;
}
