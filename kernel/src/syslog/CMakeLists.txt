file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(knl_syslog OBJECT ${SOURCES})

target_compile_options(
    knl_syslog PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_STATIC}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
)

target_link_options(
    knl_syslog PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_STATIC}>"
)
