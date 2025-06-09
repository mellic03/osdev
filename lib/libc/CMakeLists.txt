file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdio/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/*.cpp
)


# add_library(libc OBJECT ${SOURCES})
# target_link_options(libc PUBLIC ${LIBC_LINK_FLAGS})
# target_link_libraries(libc libk)


add_library(klibc OBJECT ${SOURCES})
target_link_libraries(klibc CringeOS)
target_compile_definitions(klibc PUBLIC -D__is_kernel)


# file(
#     COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
#     DESTINATION ${KERNEL_SYSROOT_DIR}/usr/
# )
