file(
    GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/string/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/*.cpp"
)

add_library(
    idk_libc STATIC
    ${SOURCES}
)


target_link_libraries(
    idk_libc
    libkernel
)

