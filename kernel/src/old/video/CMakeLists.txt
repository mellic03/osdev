
file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(
    idk_video STATIC
    ${SOURCES}
)

target_link_libraries(
    idk_video
    idk_libc
)

set(IDKERNEL_LIBRARIES ${IDKERNEL_LIBRARIES} idk_video PARENT_SCOPE)
