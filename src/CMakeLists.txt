cmake_minimum_required(VERSION 3.31)
project(MyOS LANGUAGES C ASM)

set(CMAKE_C_COMPILER i686-elf-gcc)
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O2 -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding -nostdlib -lgcc -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
# enable_language(ASM_NASM)
# set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

# Add your sources explicitly
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/boot.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel.c
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Ensure the assembly files are assembled properly
set_source_files_properties(boot.asm PROPERTIES LANGUAGE ASM)

# Target properties to indicate freestanding environment
target_compile_options(${PROJECT_NAME} PRIVATE -ffreestanding -nostdlib)
target_link_options(${PROJECT_NAME} PRIVATE -ffreestanding -nostdlib -lgcc)



# cmake_minimum_required(VERSION 3.16)
# project(CockOS LANGUAGES CXX ASM)


# # Specify the target architecture
# set(CMAKE_SYSTEM_NAME Generic)
# set(CMAKE_SYSTEM_PROCESSOR i686)

# # Specify the C++ compiler and flags
# set(CMAKE_CXX_COMPILER i686-elf-g++)
# set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")

# # Specify the linker and flags
# set(CMAKE_LINKER i686-elf-gcc)
# set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

# # Add the boot assembly file
# enable_language(ASM_NASM)
# set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

# add_library(boot OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/boot.asm)
# set_target_properties(boot PROPERTIES COMPILE_FLAGS "-f elf32")

# # Add the kernel C++ source file
# # add_library(kernel OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.cpp)

# file(
#     GLOB SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
# )


# # Create the final kernel binary
# add_executable(
#     ${PROJECT_NAME}.bin $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel>
# )


# set_target_properties(${PROJECT_NAME}.bin PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -O2 -nostdlib -lgcc")
