cmake_minimum_required(VERSION 3.25)

# Test específico para verificar soporte de __int128
project(Int128Test CXX)

# Test con diferentes estándares
set(STANDARDS 14 17 20 23)

foreach(STANDARD ${STANDARDS})
    # Test con std=c++XX
    add_executable(test_int128_cpp${STANDARD} basic_test.cpp)
    set_target_properties(test_int128_cpp${STANDARD} PROPERTIES
        CXX_STANDARD ${STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    
    # Test con std=gnu++XX (extensiones GNU habilitadas)
    add_executable(test_int128_gnu${STANDARD} basic_test.cpp)
    set_target_properties(test_int128_gnu${STANDARD} PROPERTIES
        CXX_STANDARD ${STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON  # Habilitar extensiones GNU
    )
    
    # Añadir flags específicos de GNU para habilitar __int128
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(test_int128_gnu${STANDARD} PRIVATE -std=gnu++${STANDARD})
    endif()
endforeach()

# Test con numeric_limits específico
add_executable(test_numeric_limits_int128 simple_int128_test.cpp)
set_target_properties(test_numeric_limits_int128 PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(test_numeric_limits_int128 PRIVATE -std=gnu++23)
endif()