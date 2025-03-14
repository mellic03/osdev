
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
    ck_libc
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_video PARENT_SCOPE)
