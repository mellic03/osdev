file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(knl_wm OBJECT ${SOURCES})

target_compile_options(
    knl_wm PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_ON}>"
)

target_link_options(
    knl_wm PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)
