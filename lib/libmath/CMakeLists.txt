file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)


add_library(libmath OBJECT ${SOURCES})
target_link_libraries(libmath libc++_k)
target_compile_definitions(libmath PUBLIC -D__is_kernel)

