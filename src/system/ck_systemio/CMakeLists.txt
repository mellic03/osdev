

add_library(
    ck_systemio OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/ck_systemio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ck_file-impl.cpp
)


target_link_libraries(
    ck_systemio
    ck_libc
)

