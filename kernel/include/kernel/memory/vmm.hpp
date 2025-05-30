#pragma once

#include <stdint.h>
#include <stddef.h>

namespace VMM
{
    static constexpr uint64_t PAGE_PRESENT    = 1 << 0;
    static constexpr uint64_t PAGE_WRITE      = 1 << 1;
    static constexpr uint64_t PAGE_SUPERVISOR = 1 << 2;
    static constexpr uint64_t PAGE_PWT        = 1 << 3;
    static constexpr uint64_t PAGE_PCD        = 1 << 4;
    static constexpr uint64_t PAGE_ACCESSED   = 1 << 5;
    static constexpr uint64_t PAGE_PAGESIZE   = 1 << 7;
    static constexpr uint64_t PAGE_ADDRESS    = 0x000FFFFFFFFFF000;
    static constexpr uint64_t PAGE_EXECUTE    = uint64_t(1) << uint64_t(63);

    static constexpr size_t vmFlag__None  = 0b0000;
    static constexpr size_t vmFlag__Write = 0b0001;
    static constexpr size_t vmFlag__Exec  = 0b0010;
    static constexpr size_t vmFlag__User  = 0b0100;
    struct vmObject
    {
        uintptr_t base;
        size_t    size;
        size_t    flags;
        vmObject *next;
    };


    void init();
    uint64_t *clonePML4( uint64_t *pml4 );
    // uint64_t clonePDP( uint64_t *pdp );
    // uint64_t clonePD( uint64_t *pd );

    void mapPage( uintptr_t phys, uintptr_t virt,
                  uint64_t flags=PAGE_PRESENT|PAGE_WRITE );

    void mapRange( uintptr_t phys, uintptr_t virt, size_t nbytes,
                   uint64_t flags=PAGE_PRESENT|PAGE_WRITE );

    void unmapPage( uintptr_t virt );
    // void unmapRange( uintptr_t virt, size_t nbytes );

}



struct vmm_pml4_t
{
    /// @brief Is this a valid entry?
    uint64_t present:1;
    /// @brief If clear, this page is read only;
    uint64_t writable:1;
    /// @brief If set, this page is user accessable;
    uint64_t user:1;
    /// @brief If set, write through caching is enabled
    uint64_t write_through:1;
    /// @brief Page will not be cached
    uint64_t cache_disable:1;
    /// @brief If set, this page has been recently accessed by the CPU
    uint64_t accessed:1;
    uint64_t unused_0:1;
    uint64_t reserved_0:1;
    uint64_t unused_1:4;
    /// @brief The address of the page directory pointer table
    uint64_t address:36;
    uint64_t reserved_1:4;
    uint64_t unused_2:11;
    uint64_t disable_execution:1;
} __attribute__((packed));


struct vmm_pml4e_t
{
    vmm_pml4_t *map;
};



struct vmm_pdpte_t
{
    /// @brief Is this a valid entry?
    uint64_t present:1;
    /// @brief If clear, this page is read only;
    uint64_t writable:1;
    /// @brief If set, this page is user accessable;
    uint64_t user:1;
    /// @brief If set, write through caching is enabled
    uint64_t write_through:1;
    /// @brief Page will not be cached
    uint64_t cache_disable:1;
    /// @brief If set, this page has been recently accessed by the CPU
    uint64_t accessed:1;
    uint64_t unused_0:1;
    /// @brief Set to 0
    uint64_t page_size:1;
    uint64_t unused_1:4;
    /// @brief The address of the page directory pointer table
    uint64_t address:36;
    uint64_t reserved_1:4;
    uint64_t unused_2:11;
    uint64_t disable_execution:1;
} __attribute__((packed));

struct vmm_pdpt_t
{
    vmm_pdpte_t *map;
};


struct vmm_pde_t
{
    uint64_t present       :1;
    uint64_t writable      :1;
    uint64_t user          :1;
    uint64_t write_through :1;
    uint64_t cache_disable :1;
    uint64_t accessed      :1;
    uint64_t unused_0      :1;
    uint64_t page_size     :1;
    uint64_t unused_1      :4;
    uint64_t address       :36;
    uint64_t reserved_1    :4;
    uint64_t unused_2      :11;
    uint64_t ex            :1;
} __attribute__((packed));

struct vmm_pd_t
{
    vmm_pde_t *map;
};



struct vmm_pte_t
{
    /// @brief Is this a valid entry?
    uint64_t present:1;
    /// @brief If clear, this page is read only;
    uint64_t writable:1;
    /// @brief If set, this page is user accessable;
    uint64_t user:1;
    /// @brief If set, write through caching is enabled
    uint64_t write_through:1;
    /// @brief Page will not be cached
    uint64_t cache_disable:1;
    /// @brief If set, this page has been recently accessed by the CPU
    uint64_t avalible:3;
    /// @brief The page has been written to
    uint64_t ditry:1;
    /// @brief Use Page Attribute Table
    uint64_t pat:1;
    uint64_t global:1;
    uint64_t unused_0:3;
    uint64_t address:36;
    uint64_t reserved:2;
    uint64_t unused_1:7;
    uint64_t protection_key:4;
    uint64_t execution_disable:1;
} __attribute__((packed));

struct vmm_pt_t
{
    vmm_pde_t *map;
};
