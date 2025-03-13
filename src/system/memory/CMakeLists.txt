
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(
    idk_memory STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_memory
    ck_libc
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_memory PARENT_SCOPE)
