
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/abi/icxxabi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/driver/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/*.cpp

    # ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/sse.asm
    # ${CMAKE_CURRENT_SOURCE_DIR}/gdt/gdt.cpp
)



set(IDKERNEL_SOURCES ${SOURCES} PARENT_SCOPE)
