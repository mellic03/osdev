file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(
    mouse
    ${SOURCES}
)

# target_link_libraries(
#     mouse.elf
#     tinyubsan
# )

