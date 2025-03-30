#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
    #define KERNEL_EXTERN_ASM extern "C"
#else
    #define KERNEL_EXTERN_ASM extern
#endif

// #ifdef __cplusplus
// extern "C" {
// #endif
