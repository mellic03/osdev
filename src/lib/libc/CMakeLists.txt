
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


add_library(
    ck_libc STATIC
    ${SOURCES}
)

