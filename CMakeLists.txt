cmake_minimum_required(VERSION 3.25)
project(AlgoritmiaCombinatoria VERSION 1.0.0 LANGUAGES CXX)

# Configuración del estándar C++23 con extensiones GNU habilitadas para GCC
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_EXTENSIONS ON)  # Habilitar extensiones GNU para __int128
else()
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Configuración específica para diferentes compiladores
if(MSVC)
    # Configuración para MSVC
    add_compile_options(/std:c++23 /W4 /permissive-)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # Configuración para GCC - usar gnu++23 para habilitar extensiones como __int128
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++23")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Configuración para Clang
    add_compile_options(-std=c++23 -Wall -Wextra -Wpedantic)
endif()

# Configuración de Release/Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(/Od /Zi)
    else()
        add_compile_options(-O0 -g)
    endif()
else()
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Buscar dependencias
find_package(Boost 1.80 REQUIRED)

# Configuración de vcpkg si está disponible
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(Catch2 3 CONFIG REQUIRED)
else()
    # Fallback para sistemas sin vcpkg
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# Incluir directorios
include_directories(include)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/combinatorics.cpp
)

# Archivos de cabecera
set(HEADERS
    include/combinatorics.hpp
    include/combinatorics_impl.hpp
)

# Ejecutable principal
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Ejecutable de demostración separado
add_executable(factorial_demo 
    factorial_optimization_demo.cpp
    src/combinatorics.cpp
)

# Enlazar librerías
target_link_libraries(${PROJECT_NAME} 
    Boost::boost
)

target_link_libraries(factorial_demo 
    Boost::boost
)

# Configuración específica para diferentes sistemas
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Tests
enable_testing()

add_executable(tests
    tests/test_main.cpp
    tests/test_combinatorics.cpp
    src/combinatorics.cpp
)

target_link_libraries(tests 
    Boost::boost
    Catch2::Catch2WithMain
)

# Registrar tests con CTest
include(CTest)
include(Catch)
catch_discover_tests(tests)

# Configurar instalación
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include
)