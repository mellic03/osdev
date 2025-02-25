cmake_minimum_required(VERSION 3.31)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(
    ${PROJECT_NAME} OBJECT
    ${SOURCES}
)

