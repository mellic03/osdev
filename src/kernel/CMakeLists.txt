
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/boot/requests.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/smp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/gdt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/gdt.asm

    ${CMAKE_CURRENT_SOURCE_DIR}/driver/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/driver/*.asm

    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/stub.asm

    ${CMAKE_CURRENT_SOURCE_DIR}/kshell/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kpanic/*.*
    ${CMAKE_CURRENT_SOURCE_DIR}/kvideo/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/memory/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/sde/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gx/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gx/command/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/syscall/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/video/*.cpp
)



set(IDKERNEL_SOURCES ${SOURCES} PARENT_SCOPE)
