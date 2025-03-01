if(ARCH_i686)
    add_executable(
        loader.bin
        ${CMAKE_SOURCE_DIR}/system/loader/gdt-32.cpp
        ${CMAKE_SOURCE_DIR}/system/loader/lmain.cpp
        ${CMAKE_SOURCE_DIR}/system/loader/info.cpp
        ${CMAKE_SOURCE_DIR}/system/loader/boot.asm
        ${CMAKE_SOURCE_DIR}/system/interpreter/interpreter.cpp
        ${CMAKE_SOURCE_DIR}/system/interpreter/interpreter.cpp

    )

    target_link_libraries(
        loader.bin
        /usr/lib/gcc/i686-elf/11.2.0/crtbegin.o
        ${CMAKE_SOURCE_DIR}/system/kcommon/ctor/crti.o
        stdc++-i686
        KCommon-i686
        /usr/lib/gcc/i686-elf/11.2.0/crtend.o
        ${CMAKE_SOURCE_DIR}/system/kcommon/ctor/crtn.o
    )

    
endif()
