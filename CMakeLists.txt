cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(idkernel)
ENABLE_LANGUAGE(ASM_NASM)

set(IDKERNEL_LIBRARIES "")
set(IDKERNEL_SOURCES "")

set(KERNEL_SYSROOT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/bin")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

include(${CMAKE_SOURCE_DIR}/toolchain.cmake)

include_directories("${CMAKE_SOURCE_DIR}/external/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libk/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc++/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libk/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc++/")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/kernel/")
# add_subdirectory("${CMAKE_SOURCE_DIR}/src/programs/")


add_executable(
    ${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/src/tinyubsan.cpp
    ${IDKERNEL_SOURCES}
)

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC 
    -D__is_kernel
)

# target_compile_options(
#     ${PROJECT_NAME} PRIVATE
#     $<$<COMPILE_LANGUAGE:CXX>:-fsanitize=undefined>
# )


target_link_libraries(
    ${PROJECT_NAME}
    libc_k
    libc++_k
    ${IDKERNEL_LIBRARIES} 
)


