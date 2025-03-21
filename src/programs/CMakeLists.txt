
add_executable(
    kshell
    ${CMAKE_CURRENT_SOURCE_DIR}/src/_start.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shell.cpp
)


target_link_libraries(kshell libc)

set_target_properties(
    kshell PROPERTIES
    LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
)


target_compile_options(
    kshell PUBLIC "-Wl,-ffunction-sections -Wl,-fdata-sections"
)



# target_link_options(
#     kshell PUBLIC "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
#     # kshell PUBLIC "-Ttext 0xFFFF800100000000"
# )

# target_link_options(kshell PUBLIC "-Ttext 0xFFFF800100000000")
# target_link_options(kshell PUBLIC "-Wl,--no-relax -Wl,--gc-sections")