
add_library(
    ck_memory STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/linear_allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stack_allocator.cpp
)

