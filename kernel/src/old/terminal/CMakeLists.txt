
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(
    idk_terminal STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_terminal
    idk_libc
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_terminal PARENT_SCOPE)
