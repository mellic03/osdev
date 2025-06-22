
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/assert/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/math/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/stdio/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/string/")



file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/libc.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/assert/*.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/math/*.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/stdio/*.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/*.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/string/*.cpp"
)



add_library(
    klibc STATIC ${SOURCES}
)

target_compile_options(
    klibc PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU}>"
)

target_link_options(
    klibc PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
)



# set_target_properties(
#     klibc PROPERTIES
#     COMPILE_OPTIONS "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU} ${KFLAGS_SSE_OFF}>"
#     LINK_OPTIONS    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
# )

# set_source_files_properties(
#     foo.cpp PROPERTIES
#     COMPILE_OPTIONS "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU} ${KFLAGS_SSE_OFF}>"
#     LINK_OPTIONS    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
# )


target_link_libraries(
    klibc PRIVATE
    klibc_assert
    klibc_math
    klibc_stdio
    klibc_stdlib
    klibc_string
)

