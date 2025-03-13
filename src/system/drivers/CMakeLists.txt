

add_library(
    idk_drivers STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/kb-ps2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/serial.cpp
)


target_link_libraries(
    idk_drivers
    ck_libc
    ck_libc++
)

set(IDK_LIBRARIES ${IDK_LIBRARIES} idk_drivers PARENT_SCOPE)
