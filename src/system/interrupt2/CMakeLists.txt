
add_library(
    cpu_interrupts
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stub.asm
)

