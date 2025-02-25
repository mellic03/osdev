cmake_minimum_required(VERSION 3.31)
project(CockOSlibc)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    ${ALL_LIBS}
)


