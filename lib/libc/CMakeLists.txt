file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdio/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/*.cpp
)

add_library(libc OBJECT ${SOURCES})
target_link_libraries(libc libk)


add_library(libc_k STATIC ${SOURCES})
target_link_libraries(libc_k libk)
target_compile_definitions(libc_k PUBLIC -D__is_kernel)


file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${KERNEL_SYSROOT_DIRECTORY}
)
