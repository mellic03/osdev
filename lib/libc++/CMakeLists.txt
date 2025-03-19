file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(
    idk_libc++ STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_libc++
    idk_libc
)
