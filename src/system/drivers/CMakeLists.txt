
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
    ck_libc
    ck_libc++
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_drivers PARENT_SCOPE)
