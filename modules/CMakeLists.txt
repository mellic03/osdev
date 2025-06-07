cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(keyboard)
ENABLE_LANGUAGE(ASM_NASM)

set(CringeModules_LIBRARIES "")
set(__x86_64__, "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../sysroot/drv)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmtoolchain.cmake)

include_directories("${CMAKE_SOURCE_DIR}/../external/")
include_directories("${CMAKE_SOURCE_DIR}/../lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/../lib/libc++/include/")

include_directories("${CMAKE_SOURCE_DIR}/../kernel/include/")
include_directories("${CMAKE_SOURCE_DIR}/../kernel/include/arch/x86_64/")
include_directories("${CMAKE_SOURCE_DIR}/../include/")

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/keyboard/main.cpp)

