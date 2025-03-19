
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stub.asm
)

add_library(
    idk_interrupt STATIC
    ${SOURCES}
)


target_link_libraries(
    idk_interrupt
    idk_libc
    idk_drivers
)

