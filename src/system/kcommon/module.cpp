#include "module.hpp"
#include  "multiboot2.hpp"

#include <stdc++/cstdio.hpp>

// static void
// process_tag( multiboot_tag_module *tag )
// {
//     printf("mod_start: 0x%x\n", tag->mod_start);
//     printf("mod_end:   0x%x\n", tag->mod_end);
//     printf("mod_size:  0x%d\n", tag->size);
//     printf("mod_name:  %s\n",   tag->cmdline);
// }


// void ckModulesInit( multiboot_tag *tag )
// {
//     while (tag->type != MULTIBOOT_TAG_TYPE_END)
//     {
//         if (tag->type == MULTIBOOT_TAG_TYPE_MODULE)
//         {
//             process_tag((multiboot_tag_module*)(tag));
//         }

//         tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
//     }
// }




