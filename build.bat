@echo off
setlocal enabledelayedexpansion

:: Script de construcción para Windows
:: Soporta MSVC, GCC (MinGW-w64) y Clang

:: Configuración por defecto
set "COMPILER="
set "BUILD_TYPE=Release"
set "GENERATOR=Ninja"
set "JOBS=4"
set "RUN_AFTER=false"
set "TEST_AFTER=false"
set "CLEAN_BEFORE=false"

:: Parsear argumentos de línea de comandos
:parse_args
if "%~1"=="" goto end_parse
if "%~1"=="-c" (
    set "COMPILER=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="--compiler" (
    set "COMPILER=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="-t" (
    set "BUILD_TYPE=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="--type" (
    set "BUILD_TYPE=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="-g" (
    set "GENERATOR=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="--generator" (
    set "GENERATOR=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="-j" (
    set "JOBS=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="--jobs" (
    set "JOBS=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="-r" (
    set "RUN_AFTER=true"
    shift
    goto parse_args
)
if "%~1"=="--run" (
    set "RUN_AFTER=true"
    shift
    goto parse_args
)
if "%~1"=="-T" (
    set "TEST_AFTER=true"
    shift
    goto parse_args
)
if "%~1"=="--test" (
    set "TEST_AFTER=true"
    shift
    goto parse_args
)
if "%~1"=="-C" (
    set "CLEAN_BEFORE=true"
    shift
    goto parse_args
)
if "%~1"=="--clean" (
    set "CLEAN_BEFORE=true"
    shift
    goto parse_args
)
if "%~1"=="-h" (
    goto show_help
)
if "%~1"=="--help" (
    goto show_help
)
echo Opción desconocida: %~1
goto show_help

:end_parse

echo [INFO] Iniciando construcción del proyecto AlgoritmiaCombinatoria

:: Limpiar si se solicita
if "%CLEAN_BEFORE%"=="true" (
    echo [INFO] Limpiando directorio de construcción...
    if exist build rmdir /s /q build
)

:: Crear directorio de construcción
if not exist build mkdir build
cd build

:: Configurar variables de CMake
set "CMAKE_ARGS=-G %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%"

:: Configurar PATH para MSYS2
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

:: Configurar compilador
if "%COMPILER%"=="gcc" (
    echo [INFO] Usando MSYS2 GCC
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe"
) else if "%COMPILER%"=="clang" (
    echo [INFO] Usando MSYS2 Clang
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/clang.exe"
) else if "%COMPILER%"=="msvc" (
    echo [INFO] Usando MSVC
    set "GENERATOR=Visual Studio 17 2022"
    set "CMAKE_ARGS=-G %GENERATOR% -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE%"
) else (
    echo [INFO] Usando MSYS2 GCC por defecto
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe"
)

:: Verificar si vcpkg está disponible
if defined VCPKG_ROOT (
    echo [INFO] Usando vcpkg en: %VCPKG_ROOT%
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
)

:: Configurar CMake
echo [INFO] Configurando proyecto con CMake...
cmake .. %CMAKE_ARGS%
if errorlevel 1 (
    echo [ERROR] Error en la configuración de CMake
    exit /b 1
)

:: Construir
echo [INFO] Construyendo proyecto...
cmake --build . --config %BUILD_TYPE% --parallel %JOBS%
if errorlevel 1 (
    echo [ERROR] Error en la construcción
    exit /b 1
)

echo [SUCCESS] Construcción completada exitosamente

:: Ejecutar pruebas si se solicita
if "%TEST_AFTER%"=="true" (
    echo [INFO] Ejecutando pruebas...
    ctest --output-on-failure --parallel %JOBS%
    if errorlevel 1 (
        echo [ERROR] Las pruebas fallaron
        exit /b 1
    )
    echo [SUCCESS] Todas las pruebas pasaron
)

:: Ejecutar proyecto si se solicita
if "%RUN_AFTER%"=="true" (
    echo [INFO] Ejecutando proyecto...
    if exist "%BUILD_TYPE%\AlgoritmiaCombinatoria.exe" (
        "%BUILD_TYPE%\AlgoritmiaCombinatoria.exe"
    ) else if exist "AlgoritmiaCombinatoria.exe" (
        "AlgoritmiaCombinatoria.exe"
    ) else (
        echo [ERROR] No se encontró el ejecutable
        exit /b 1
    )
)

echo [SUCCESS] ¡Proceso completado!
goto end

:show_help
echo Uso: %0 [OPCIONES]
echo.
echo Opciones:
echo   -c, --compiler COMP    Especificar compilador (gcc, clang, msvc)
echo   -t, --type TYPE        Tipo de construcción (Debug, Release)
echo   -g, --generator GEN    Generador de CMake (Ninja, "Visual Studio 17 2022", etc.)
echo   -j, --jobs JOBS        Número de trabajos paralelos
echo   -r, --run              Ejecutar después de construir
echo   -T, --test             Ejecutar pruebas después de construir
echo   -C, --clean            Limpiar antes de construir
echo   -h, --help             Mostrar esta ayuda
echo.
echo Ejemplos:
echo   %0 -c gcc -t Release -g Ninja
echo   %0 --compiler msvc --type Debug --test
echo   %0 --clean --run

:end