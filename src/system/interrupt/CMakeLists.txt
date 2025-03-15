
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stub.asm
)

add_library(
    idk_interrupt STATIC
    ${SOURCES}
)


target_link_libraries(
    idk_interrupt
    ck_libc
)


set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_interrupt PARENT_SCOPE)
