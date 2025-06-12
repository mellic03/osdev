file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(
    klibc++ STATIC ${SOURCES}
)

target_compile_options(
    klibc++ PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

target_link_options(
    klibc++ PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)


target_link_libraries(
    klibc++ PRIVATE
    klibc
)

