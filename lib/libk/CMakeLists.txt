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


file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${KERNEL_SYSROOT_DIRECTORY}
)
