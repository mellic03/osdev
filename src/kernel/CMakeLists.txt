
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/_start.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ksystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/module.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/abi/icxxabi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/gdt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/gdt.asm

    ${CMAKE_CURRENT_SOURCE_DIR}/driver/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/stub.asm


    ${CMAKE_CURRENT_SOURCE_DIR}/lang/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/syscall/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/sse.asm
    # ${CMAKE_CURRENT_SOURCE_DIR}/gdt/gdt.cpp
)



set(IDKERNEL_SOURCES ${SOURCES} PARENT_SCOPE)
