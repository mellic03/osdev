
add_library(
    libmath STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libmath.cpp"
)

target_compile_options(
    libmath PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

target_link_options(
    libmath PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)




add_library(
    ulibmath SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/libmath.cpp"
)

target_compile_options(
    ulibmath PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

