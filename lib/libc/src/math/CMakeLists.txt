file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
)

add_library(klibc_math OBJECT ${SOURCES})

target_compile_options(
    klibc_math PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU}>"
)

target_link_options(
    klibc_math PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
)

