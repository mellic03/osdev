
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


add_library(
    idk_drivers STATIC
    ${SOURCES}
)


target_link_libraries(
    idk_drivers
    idk_libc
    idk_libc++
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_drivers PARENT_SCOPE)
