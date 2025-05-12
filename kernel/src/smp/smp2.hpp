// #pragma once

// #include <stdbool.h>
// #include <stdint.h>

// #define SMP_TRAMPOLINE_BLOB_ADDR        0x1000
// #define SMP_AP_BOOT_COUNTER_ADDR        0xff0

// #define SMP_TRAMPOLINE_ARG_IDTPTR       0xfa0
// #define SMP_TRAMPOLINE_ARG_RSP          0xfb0
// #define SMP_TRAMPOLINE_ARG_ENTRYPOINT   0xfc0
// #define SMP_TRAMPOLINE_ARG_CR3          0xfd0
// #define SMP_TRAMPOLINE_ARG_CPUINFO      0xfe0

// #define CPU_MAX                         256

// /* TODO: If stack size is set to PAGE_SIZE * 32, there will be some #PF
//  * exceptions in userspace apps (specifically in hansh). From the contexts, it
//  * seems that the stack is corrupted. But we do not know the reason. In the
//  * future, we should deeply dive into this. (Dec 23, 2023)
//  */
// #define STACK_SIZE                      (PAGE_SIZE * 64)

// struct tss_t
// {
//     uint32_t reserved;
//     uint64_t rsp0;
//     uint64_t rsp1;
//     uint64_t rsp2;

//     uint32_t reserved_1;
//     uint32_t reserved_2;

//     uint64_t ist1;
//     uint64_t ist2;
//     uint64_t ist3;
//     uint64_t ist4;
//     uint64_t ist5;
//     uint64_t ist6;
//     uint64_t ist7;

//     uint64_t reserved_3;
//     uint16_t reserved_4;
//     uint16_t io_bitmap_offset;
// } __attribute__((packed));

// /*
//  * In 64bit and with smp, there is a local structure for each cpu stored in the
//  * gs register (other kernels can use fs). This structure contains a temporary
//  * stack for the syscall, an address to store the process stack temporarily.
//  */
// struct cpu_t
// {
//     int64_t errno;
//     tss_t tss;
//     uint16_t cpu_id;
//     uint16_t lapic_id;
//     bool is_bsp;
//     uint64_t fpu_storage_size;
//     void (*fpu_save)(void *);
//     void (*fpu_restore)(void *);
//     uint16_t proc_id;
//     uint8_t reserved;
// }  __attribute__((packed));;

// struct SMPInfo
// {
//     cpu_t cpus[CPU_MAX];
//     uint16_t num_cpus;
// };


// namespace SMP
// {
//     void     init(void);
//     SMPInfo *getInfo(void);
//     cpu_t   *this_cpu(bool force_read);
//     bool     set_errno(int64_t val);
//     void     cpu_debug(void);
// }