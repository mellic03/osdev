

add_library(
    ck_drivers STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ps2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/serial.cpp
)


target_link_libraries(
    ck_drivers
    ck_libc
    ck_libc++
)

