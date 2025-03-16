
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
    idk_libc
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_memory PARENT_SCOPE)
