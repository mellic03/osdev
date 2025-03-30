file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/_start.asm
    # ${CMAKE_SOURCE_DIR}/src/kernel/abi/cxxabi.cpp
)

set(
    USERPROGRAM_FLAGS
    -Wl,-m,elf_x86_64
    -Wl,--build-id=none
    -static
    -nostdlib
    -nodefaultlibs
    -nostartfiles
    -zmax-page-size=0x1000
    -mcmodel=large
    -mno-red-zone
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
)


# add_executable(kshell.elf ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/kshell.cpp)
# target_link_libraries(kshell.elf PUBLIC libk libc libc++)
# target_link_options(kshell.elf PUBLIC ${USERPROGRAM_FLAGS})


add_executable(makewin.elf ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/makewin.cpp)
target_link_libraries(makewin.elf PUBLIC libk libc libc++)
target_link_options(makewin.elf PUBLIC ${USERPROGRAM_FLAGS})


# add_executable(sum.elf ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/sum.cpp)
# target_link_libraries(sum.elf PUBLIC libk libc libc++)
# target_link_options(sum.elf PUBLIC ${USERPROGRAM_FLAGS})


# target_compile_options(
#     makewin.elf PRIVATE
#     $<$<COMPILE_LANGUAGE:CXX>:-mcmodel=large>
#     $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
#     $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
# )
