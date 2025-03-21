include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm
)

add_library(
    libk STATIC
    ${SOURCES}
)


file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${KERNEL_SYSROOT_DIRECTORY}
)
