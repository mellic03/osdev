file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/x86_64/acpi/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/x86_64/apic/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cringe/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/driver/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/driver/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interrupt/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ipc/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/syscall/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/smp/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sym/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wm/*.cpp


)

set(CringeOS_SOURCES ${SOURCES} PARENT_SCOPE)


# file(
#     COPY ${CMAKE_CURRENT_SOURCE_DIR}/include
#     DESTINATION ${KERNEL_SYSROOT_DIR}/usr/
# )
