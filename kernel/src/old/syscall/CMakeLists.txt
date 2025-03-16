
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(
    idk_syscall STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_syscall
    idk_libc
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_syscall PARENT_SCOPE)
