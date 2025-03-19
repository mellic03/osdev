#include <kmalloc.h>

#include <limine/limine.h>
#include <kdriver/serial.hpp>
#include <kinplace/inplace_stack.hpp>
#include <kinplace/inplace_vector.hpp>
#include <kmemory/memory.hpp>

using namespace idk;

static linear_allocator kmem0;
static buddy_allocator  kalloc0;


void *kmalloc( size_t size )
{
    return kalloc0.alloc(size, alignof(uint64_t));
}


void kfree( void *ptr )
{
    kalloc0.free(ptr);
}


void kmalloc_init( void *base, uint64_t nbytes )
{
    kmem0   = linear_allocator(base, nbytes);
    kalloc0 = buddy_allocator(kmem0);

    // return KRET_SUCCESS;
}






// void load_kmmaps( uint64_t, uint64_t );
// void sort_kmmaps();


// static MemoryMap kmmap_buf[32];
// static inplace_vector<MemoryMap> kmmaps;

// kret_t kmalloc_init( uint64_t hhdm, uint64_t addr,
//                      inplace_vector<MemoryMap> **maps_out )
// {
//     SYSLOG_BEGIN("kmalloc_init");

//     kmmaps = inplace_vector<MemoryMap>(32, kmmap_buf);
//     load_kmmaps(hhdm, addr);
//     sort_kmmaps();

//     for (auto &map: kmmaps)
//     {
//         SYSLOG("0x%lx", map.addr, map.len, map.len/idk::MEGA);
//         SYSLOG("\t%luB",  map.len);
//         SYSLOG("\t%luKB", map.len/idk::KILO);
//         SYSLOG("\t%luMB", map.len/idk::MEGA);
//     }

//     kmem0   = linear_allocator(kmmaps[0].len, (void*)(kmmaps[0].addr));
//     kalloc0 = buddy_allocator(kmem0);
//     // SYSLOG("kmmaps.size(): %lu", kmmaps.size());

//     if (maps_out)
//     {
//         *maps_out = &kmmaps;
//     }

//     SYSLOG_END();
//     return KRET_SUCCESS;
// }



// void load_kmmaps( uint64_t hhdm, uint64_t addr )
// {
//     auto *res = reinterpret_cast<limine_memmap_response*>(addr);

//     for (uint64_t i=0; i<res->entry_count; i++)
//     {
//         auto *e = res->entries[i];
//         if (e->type == LIMINE_MEMMAP_USABLE)
//         {
//             kmmaps.push_back(
//                 MemoryMap(hhdm+e->base, e->length)
//             );
//         }
//     }
// }


// void sort_kmmaps()
// {
//     for (size_t i=0; i<kmmaps.size(); i++)
//     {
//         for (size_t j=i+1; j<kmmaps.size(); j++)
//         {
//             if (kmmaps[i].len < kmmaps[j].len)
//             {
//                 auto tmp = kmmaps[i];
//                 kmmaps[i] = kmmaps[j];
//                 kmmaps[j] = tmp;
//             }
//         }
//     }
// }
