add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/drivers/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/idk_sysio/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/interrupt/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/memory/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/syscall/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/terminal/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/video/)


file(
    GLOB SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sse.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/gdt/gdt.cpp
)


add_library(
    idk_system OBJECT
    ${SOURCES}
)

target_link_libraries(
    idk_system
    ${IDK_LIBRARIES}
)
