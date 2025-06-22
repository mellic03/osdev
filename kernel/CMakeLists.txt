
file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/kmain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/init.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tinyubsan.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/keyboard.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mouse.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/arch/x86_64/acpi/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/arch/x86_64/apic/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cringe/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/driver/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/driver/*.asm"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interrupt/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ipc/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/memory/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/syscall/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/smp/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/sym/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/sys/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp"
)


add_executable(
    ${PROJECT_NAME} ${SOURCES}
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_COMPILE_KERNEL_NOFPU}>"
)

target_link_options(
    ${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${KFLAGS_LINK_KERNEL}>"
)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/cpu")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/syslog")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/wm")

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    klibc
    klibc++
    knl_cpu
    knl_syslog
    knl_wm
)
