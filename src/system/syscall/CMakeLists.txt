
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
    ck_libc
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_syscall PARENT_SCOPE)
