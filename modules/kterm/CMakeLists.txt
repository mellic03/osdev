file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(
    kterm.elf
    ${SOURCES}
)

target_link_libraries(
    kterm.elf
    tinyubsan
)

