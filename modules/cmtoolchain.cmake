include(${CMAKE_SOURCE_DIR}/../toolchain-common.cmake)

set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/../external/x86_64-elf-tools-linux/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/../external/x86_64-elf-tools-linux/bin/x86_64-elf-g++)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/../external/x86_64-elf-tools-linux)


set(CringeModules_C_CXX_FLAGS
    "${CringeOS_COMMON_CXX_FLAGS} \
    -fPIE -pie \
    \
    -T ${CMAKE_SOURCE_DIR}/linker.ld \
    -mcmodel=small"
)

set(CMAKE_C_FLAGS
    "-std=c11 \
    ${CringeModules_C_CXX_FLAGS}"
)

set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    ${CringeModules_C_CXX_FLAGS} \
    -include ${CMAKE_SOURCE_DIR}/../lib/libc++/include/new \
    -fno-rtti"
)

set(CMAKE_ASM_NASM_FLAGS
    "-Wall \
    -f elf64"
)
