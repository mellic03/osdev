include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(
    libkernel STATIC
    ${SOURCES}
)
