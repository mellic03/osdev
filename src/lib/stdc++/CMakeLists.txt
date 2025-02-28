# if(ARCH_i686)
#     set(KS_STDCPP_NAME stdc++-i686)
# else()
#     set(KS_STDCPP_NAME stdc++-x86_64)
# endif()



add_library(
    # ${KS_STDCPP_NAME} OBJECT
    stdc++-${KS_ARCH} OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/cstdio-file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cstdio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cstdlib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cstring.cpp
)


