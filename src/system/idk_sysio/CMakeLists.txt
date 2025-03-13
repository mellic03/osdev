

add_library(
    idk_systemio STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/idk_sysio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/idk_file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sysio_syscall.cpp
)


target_link_libraries(
    idk_systemio
    idk_memory
    ck_libc
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_systemio PARENT_SCOPE)
