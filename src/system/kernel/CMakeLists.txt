if(ARCH_x86_64)
    add_executable(
        kernel.bin
        ${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp
    )

    target_link_libraries(
        kernel.bin
        # /usr/lib/gcc/i686-elf/11.2.0/crtbegin.o
        # ${CMAKE_SOURCE_DIR}/src/kernel/ctor/crti.o
        stdc++-x86_64
        KCommon-x86_64
        # /usr/lib/gcc/i686-elf/11.2.0/crtend.o
        # ${CMAKE_SOURCE_DIR}/src/kernel/ctor/crtn.o
    )

endif()
