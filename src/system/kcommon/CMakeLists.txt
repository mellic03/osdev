
add_library(
    KCommon-${KS_ARCH} OBJECT
    ${CMAKE_SOURCE_DIR}/system/kcommon/ctor/icxxabi.cpp
    ${CMAKE_SOURCE_DIR}/system/kcommon/file/bmp.cpp
    ${CMAKE_SOURCE_DIR}/system/kcommon/terminal/terminal.cpp
    ${CMAKE_SOURCE_DIR}/system/kcommon/terminal/ckterminal.cpp
    ${CMAKE_SOURCE_DIR}/system/drivers/serial.cpp
    ${CMAKE_SOURCE_DIR}/system/drivers/ps2.cpp
)


target_link_libraries(
    KCommon-${KS_ARCH}
    ck_libc
)
