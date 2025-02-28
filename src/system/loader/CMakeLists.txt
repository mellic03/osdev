if(ARCH_i686)
    add_executable(
        loader.bin
        ${CMAKE_SOURCE_DIR}/system/loader/gdt-32.cpp
        ${CMAKE_SOURCE_DIR}/system/loader/lmain.cpp
        ${CMAKE_SOURCE_DIR}/system/loader/boot.asm
    )

    target_link_libraries(
        loader.bin
        stdc++-i686
        KCommon-i686
    )

    
endif()
