file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
)

add_library(klibc_stdlib OBJECT ${SOURCES})

target_compile_options(
    klibc_stdlib PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU}>"
)

target_link_options(
    klibc_stdlib PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
)




# add_library(
#     ulibc_stdlib OBJECT
#     ${SOURCES}
# )

# target_compile_options(
#     ulibc_stdlib PRIVATE
#     "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_SHARED}>"
#     "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
# )

# target_link_options(
#     ulibc_stdlib PRIVATE
#     "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_SHARED}>"
# )
