file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# add_library(libc++ OBJECT ${SOURCES})
# target_link_libraries(libc++ libc)


add_library(klibc++ OBJECT ${SOURCES})
target_link_libraries(klibc++ klibc)
target_compile_definitions(klibc++ PUBLIC -D__is_kernel)


# target_link_options(
#     libc++ PUBLIC
#     -Wl,-m,elf_x86_64
#     -Wl,--build-id=none
#     -static
#     -nostdlib
#     -nodefaultlibs
#     -nostartfiles
#     -zmax-page-size=0x1000
#     -mno-red-zone
#     -mcmodel=large
#     -T${CMAKE_SOURCE_DIR}/src/linker.ld
# )


# target_link_options(
#     klibc++ PUBLIC
#     -Wl,-m,elf_x86_64
#     -Wl,--build-id=none
#     -static
#     -nostdlib
#     -nodefaultlibs
#     -nostartfiles
#     -zmax-page-size=0x1000
#     -mno-red-zone
#     -mcmodel=large
#     -T${CMAKE_SOURCE_DIR}/src/linker.ld
# )


# file(
#     COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
#     DESTINATION ${KERNEL_SYSROOT_DIR}/usr/
# )

