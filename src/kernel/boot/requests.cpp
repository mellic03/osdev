#include "boot.hpp"

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);


__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request lim_hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request lim_addr_req = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request lim_fb_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};


__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request lim_module_req = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request lim_mmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request lim_mp_req = {
    .id = LIMINE_MP_REQUEST,
    .revision = 0
};


__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;


// const char *mmap_str( uint32_t type )
// {
//     switch (type)
//     {
//         default: return "UNKNOWN_TYPE";
//         case LIMINE_MEMMAP_USABLE: return "LIMINE_MEMMAP_USABLE";
//         case LIMINE_MEMMAP_RESERVED: return "LIMINE_MEMMAP_RESERVED";
//         case LIMINE_MEMMAP_ACPI_RECLAIMABLE: return "LIMINE_MEMMAP_ACPI_RECLAIMABLE";
//         case LIMINE_MEMMAP_ACPI_NVS: return "LIMINE_MEMMAP_ACPI_NVS";
//         case LIMINE_MEMMAP_BAD_MEMORY: return "LIMINE_MEMMAP_BAD_MEMORY";
//         case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: return "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE";
//         case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:  return "LIMINE_MEMMAP_EXECUTABLE_AND_MODULES";
//         case LIMINE_MEMMAP_FRAMEBUFFER: return "LIMINE_MEMMAP_FRAMEBUFFER";
//     }
// }


struct LimineRes
{
    uint64_t                      hhdm;
    limine_framebuffer_response  *fb;
    limine_executable_address_response *addr;
    limine_module_response       *modules;
    limine_memmap_response       *mmaps;
    limine_mp_response           *mp;
};
