
add_library(
    ck_libc++ STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/cstdio-file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cstdio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cstdlib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cstring.cpp
)


target_link_libraries(
    ck_libc++
    ck_libc
)
