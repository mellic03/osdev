
add_library(
    libmath STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libmath.cpp"
)

target_compile_options(
    libmath PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU}>"
)

target_link_options(
    libmath PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
)




# add_library(
#     ulibmath SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/libmath.cpp"
# )

# target_compile_options(
#     ulibmath PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_SSE_OFF}>"
# )

