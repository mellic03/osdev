
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/task.cpp
)

add_library(
    idk_task STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_task
    ${CMAKE_CURRENT_SOURCE_DIR}/switch.o
    ck_libc
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_task PARENT_SCOPE)
