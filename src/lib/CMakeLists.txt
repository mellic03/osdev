# if(ARCH_i686)
#     set(KS_STDCPP_NAME stdc++-i686)
# else()
#     set(KS_STDCPP_NAME stdc++-x86_64)
# endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libc/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libc++/)


