file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdio/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/*.cpp
)


add_library(libc OBJECT ${SOURCES})
target_link_options(libc PUBLIC ${LIBC_LINK_FLAGS})
target_link_libraries(libc libk)


add_library(libc_k STATIC ${SOURCES})
target_link_libraries(libc_k libk)
target_compile_definitions(libc_k PUBLIC -D__is_kernel)


target_link_options(
    libc PUBLIC
    -Wl,-m,elf_x86_64
    -Wl,--build-id=none
    -static
    -nostdlib
    -nodefaultlibs
    -nostartfiles
    -zmax-page-size=0x1000
    -mno-red-zone
    -mcmodel=large
    -T${CMAKE_SOURCE_DIR}/src/linker.ld
)


target_link_options(
    libc_k PUBLIC
    -Wl,-m,elf_x86_64
    -Wl,--build-id=none
    -static
    -nostdlib
    -nodefaultlibs
    -nostartfiles
    -zmax-page-size=0x1000
    -mno-red-zone
    -mcmodel=large
    -T${CMAKE_SOURCE_DIR}/src/linker.ld
)




file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${KERNEL_SYSROOT_DIRECTORY}
)
