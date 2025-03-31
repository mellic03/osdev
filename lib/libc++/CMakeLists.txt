file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(libc++ OBJECT ${SOURCES})
target_link_libraries(libc++ libc)


add_library(libc++_k STATIC ${SOURCES})
target_link_libraries(libc++_k libc_k)
target_compile_definitions(libc++_k PUBLIC -D__is_kernel)


target_link_options(
    libc++ PUBLIC
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
    libc++_k PUBLIC
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

