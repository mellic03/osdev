
add_library(
    klibc STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libc.cpp"
)

target_compile_options(
    klibc PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

target_link_options(
    klibc PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/assert/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/math/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/stdio/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/string/")

target_link_libraries(
    klibc PRIVATE
    klibc_assert
    klibc_math
    klibc_stdio
    klibc_stdlib
    klibc_string
)

