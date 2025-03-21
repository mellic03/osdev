file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(
    libc++ STATIC
    ${SOURCES}
)

target_link_libraries(
    libc++
    libc
)


file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${KERNEL_SYSROOT_DIRECTORY}
)