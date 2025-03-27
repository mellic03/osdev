file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/_start.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kshell.cpp
)


set(
    USERPROGRAM_FLAGS
    -Wl,-m,elf_x86_64
    -Wl,--build-id=none
    -nostdlib
    -static
    -zmax-page-size=0x1000
    -Wl,--no-relax
    -Wl,--gc-sections
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
)


add_executable(kshell.elf ${SOURCES})
target_link_libraries(kshell.elf PUBLIC libk libc)
target_link_options(kshell.elf PUBLIC ${USERPROGRAM_FLAGS})

