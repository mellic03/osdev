file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(
    keyboard.elf
    ${SOURCES}
)

# target_link_libraries(
#     keyboard.elf
#     tinyubsan
# )

