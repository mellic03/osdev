
if(ARCH_i686)
    set(KS_STDC_NAME stdc-i686)
else()
    set(KS_STDC_NAME stdc-x86_64)
endif()


file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_library(
    ${KS_STDC_NAME} OBJECT
    ${SOURCES}
)

target_link_libraries(
    ${KS_STDC_NAME}
)

# set_target_properties(
#     ${KS_STDC_NAME} PROPERTIES
#         LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker-${KS_ARCH}.ld -ffreestanding -O2 -nostdlib -lgcc"
# )

