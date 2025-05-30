cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(mouse)
ENABLE_LANGUAGE(ASM_NASM)

set(CringeModules_LIBRARIES "")
# set(CringeOS_SOURCES "")

set(__x86_64__, "")
# set(KERNEL_SYSROOT_DIR ${CMAKE_SOURCE_DIR}/sysroot)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KERNEL_SYSROOT_DIR}/usr/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KERNEL_SYSROOT_DIR}/usr/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../sysroot/drv)
# set(CMAKE_STATIC_LIBRARY_PREFIX "")
# set(CMAKE_SHARED_LIBRARY_PREFIX "")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmtoolchain.cmake)

include_directories("${CMAKE_SOURCE_DIR}/../external/")
include_directories("${CMAKE_SOURCE_DIR}/../lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/../lib/libc++/include/")

include_directories("${CMAKE_SOURCE_DIR}/../kernel/include/")
include_directories("${CMAKE_SOURCE_DIR}/../kernel/include/arch/x86_64/")
include_directories("${CMAKE_SOURCE_DIR}/../include/")

# add_subdirectory("${CMAKE_SOURCE_DIR}/../lib/libc/")
# add_subdirectory("${CMAKE_SOURCE_DIR}/../lib/libc++/")


# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mouse)
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/mouse/main.cpp)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/daemon)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/keyboard)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/achi)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/kterm)


file(
    GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/../kernel/src/cpu/*.cpp"
    "${CMAKE_SOURCE_DIR}/../kernel/src/cpu/*.asm"

    # "${CMAKE_SOURCE_DIR}/../kernel/src/cringe/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/driver/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/driver/*.asm"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/filesystem/*.cpp"
    "${CMAKE_SOURCE_DIR}/../kernel/src/gui/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/interrupt/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/kernel/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/memory/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/syscall/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/smp/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/sym/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/arch/x86_64/acpi/*.cpp"
    # "${CMAKE_SOURCE_DIR}/../kernel/src/arch/x86_64/apic/*.cpp"
)

# add_library( tubsan OBJECT "${CMAKE_SOURCE_DIR}/tinyubsan.cpp" )
# add_library( knlgui OBJECT ${SOURCES} )
# target_link_libraries( daemon knlgui )

