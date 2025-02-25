cmake_minimum_required(VERSION 3.15)
project(CockOS)

ENABLE_LANGUAGE(ASM)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(TOOLS $ENV{HOME}/opt/cross)
set(CMAKE_C_COMPILER /bin/i686-elf-gcc)
set(CMAKE_CXX_COMPILER /bin/i686-elf-g++)
set(CMAKE_ASM_COMPILER /bin/i686-elf-as)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra")
set(KERNEL_BIN ${PROJECT_NAME}.bin)
set(KERNEL_ISO ${PROJECT_NAME}.iso)



# ## A simple lib to test C compilation and linking
# add_library(mylib mylib.c)

# Compile boot.asm manually since CMake is being cringe about it
# add_custom_command(
#     TARGET ${KERNEL_BIN} PRE_BUILD
#     COMMAND nasm -felf32 ${CMAKE_SOURCE_DIR}/src/kernel/boot.asm -o ${CMAKE_SOURCE_DIR}/src/kernel/boot.o
# )

# # Generate iso file after builing kernel
# add_custom_command(
#     TARGET ${KERNEL_BIN} POST_BUILD
#     COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/generate-iso.sh ${CMAKE_SOURCE_DIR} ${PROJECT_NAME}
# )


# Include directories
# ----------------------------------------------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/src/kernel/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/lib/)
# ----------------------------------------------------------------------------------------------




# Build kernel
# ----------------------------------------------------------------------------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/libc/)


add_executable(
    ${KERNEL_BIN}
    ${CMAKE_SOURCE_DIR}/src/kernel/boot.o
    ${CMAKE_SOURCE_DIR}/src/kernel/kernel.cpp
    ${CMAKE_SOURCE_DIR}/src/kernel/terminal/terminal.c
)

target_link_libraries(
    ${KERNEL_BIN}
    libc
)


set_target_properties(
    ${KERNEL_BIN} PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/src/kernel/linker.ld -ffreestanding -O2 -nostdlib -lgcc"
)

# ----------------------------------------------------------------------------------------------





# execute_process(
#     COMMAND bash qemu-system-i386 -kernel ${CMAKE_SOURCE_DIR}/output/CockOS.bin
# )

# add_custom_command(
#     TARGET ${KERNEL_BIN} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
#     COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_BIN} ${CMAKE_SOURCE_DIR}/bin/
# )

## Generate ISO file
# add_custom_target(
#     ${KERNEL_ISO}
#     COMMAND ${CMAKE_SOURCE_DIR}/scripts/generate-iso.sh
#     COMMENT "Generating the kernel bootable iso file"
#     BYPRODUCTS ${KERNEL_ISO}
# )

# add_dependencies(${PROJECT_NAME}.iso ${PROJECT_NAME}.bin)


# add_custom_command(
#     TARGET ${KERNEL_ISO} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
#     COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_ISO} ${CMAKE_SOURCE_DIR}/bin/
# )