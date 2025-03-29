file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(libc++ OBJECT ${SOURCES})
target_link_libraries(libc++ libc)


add_library(libc++_k STATIC ${SOURCES})
target_link_libraries(libc++_k libc_k)
target_compile_definitions(libc++_k PUBLIC -D__is_kernel)


file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${KERNEL_SYSROOT_DIRECTORY}
)

