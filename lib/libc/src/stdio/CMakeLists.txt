file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(klibc_stdio OBJECT ${SOURCES})

target_compile_options(
    klibc_stdio PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

target_link_options(
    klibc_stdio PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)

