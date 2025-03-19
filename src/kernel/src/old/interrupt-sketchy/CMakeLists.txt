
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt.cpp
)

add_library(
    idk_interrupt STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_interrupt
    idk_libc
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_interrupt PARENT_SCOPE)
