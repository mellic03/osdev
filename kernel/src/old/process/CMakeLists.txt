
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(
    idk_process STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_process
    idk_drivers
    idk_libc
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_process PARENT_SCOPE)
