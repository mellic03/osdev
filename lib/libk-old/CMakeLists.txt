include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/*.cpp
)

add_library(
    libk OBJECT
    ${SOURCES}
)

# target_link_options(
#     libk PRIVATE
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
#     DESTINATION ${KERNEL_SYSROOT_DIR}
# )
