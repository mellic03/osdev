file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
)

add_library(knl_cpu OBJECT ${SOURCES})

target_compile_options(
    knl_cpu PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

target_link_options(
    knl_cpu PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)

