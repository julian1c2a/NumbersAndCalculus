@echo off
setlocal enabledelayedexpansion

rem Script de construcción mejorado para AlgoritmiaCombinatoria
rem Soporte para modo simple: build.bat [compilador] [tipo]

rem Check for help first
if "%~1"=="-h" goto show_help
if "%~1"=="--help" goto show_help

rem Modo simple: detectar si son argumentos simples
set "SIMPLE_MODE=false"
set "COMPILER="
set "BUILD_TYPE=Release"
set "CPP_STANDARD=17"

rem Verificar modo simple
if "%~1"=="gcc" (
    set "SIMPLE_MODE=true"
    set "COMPILER=gcc"
    if "%~2"=="debug" set "BUILD_TYPE=Debug"
    if "%~2"=="release" set "BUILD_TYPE=Release"
    if "%~2"=="" set "BUILD_TYPE=Release"
    call :set_cpp_standard "%~3"
    echo [INFO] Modo simple: GCC %BUILD_TYPE% C++%CPP_STANDARD%
    goto start_build
)

if "%~1"=="clang" (
    set "SIMPLE_MODE=true"
    set "COMPILER=clang"
    if "%~2"=="debug" set "BUILD_TYPE=Debug"
    if "%~2"=="release" set "BUILD_TYPE=Release"
    if "%~2"=="" set "BUILD_TYPE=Release"
    call :set_cpp_standard "%~3"
    echo [INFO] Modo simple: Clang %BUILD_TYPE% C++%CPP_STANDARD%
    goto start_build
)

if "%~1"=="msvc" (
    set "SIMPLE_MODE=true"
    set "COMPILER=msvc"
    if "%~2"=="debug" set "BUILD_TYPE=Debug"
    if "%~2"=="release" set "BUILD_TYPE=Release"
    if "%~2"=="" set "BUILD_TYPE=Release"
    call :set_cpp_standard "%~3"
    echo [INFO] Modo simple: MSVC %BUILD_TYPE% C++%CPP_STANDARD%
    goto start_build
)

rem Si no es modo simple, usar valores por defecto
if "%~1"=="" (
    set "COMPILER=gcc"
    set "BUILD_TYPE=Release"
    set "CPP_STANDARD=17"
    echo [INFO] Usando configuración por defecto: GCC Release C++17
    goto start_build
)

rem Si llegamos aquí, mostrar error
echo [ERROR] Argumentos no reconocidos: %*
echo.
goto show_help

:start_build
echo.
echo ================================================
echo         AlgoritmiaCombinatoria Builder        
echo               Modo Simple v2.0                
echo ================================================
echo.
echo Compilador: %COMPILER%
echo Build Type: %BUILD_TYPE%
echo Estandar C++: %CPP_STANDARD%
echo.

rem Limpiar build anterior si existe
if exist build (
    echo [INFO] Limpiando build anterior...
    rmdir /s /q build
)

rem Crear directorio build
mkdir build
cd build

rem Configurar PATH para MSYS2
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

rem Configurar argumentos de CMake
set "CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD%"

if "%COMPILER%"=="gcc" (
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe"
    echo [INFO] Configurando para GCC con C++%CPP_STANDARD%
)

if "%COMPILER%"=="clang" (
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/clang.exe"
    echo [INFO] Configurando para Clang con C++%CPP_STANDARD%
)

if "%COMPILER%"=="msvc" (
    set CMAKE_ARGS=-G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD%
    echo [INFO] Configurando para MSVC con C++%CPP_STANDARD%
)

rem Ejecutar CMake
echo [INFO] Configurando proyecto...
cmake .. %CMAKE_ARGS%
if errorlevel 1 (
    echo [ERROR] Error en la configuración
    cd ..
    exit /b 1
)

rem Construir
echo [INFO] Construyendo...
cmake --build . --config %BUILD_TYPE% --parallel 4
if errorlevel 1 (
    echo [ERROR] Error en la construcción
    cd ..
    exit /b 1
)

echo.
echo [SUCCESS] Construccion completada!
echo.

rem Mostrar ejecutables
echo Ejecutables generados:
if exist "*.exe" dir /b *.exe 2>nul
if exist "%BUILD_TYPE%\" if exist "%BUILD_TYPE%\*.exe" dir /b "%BUILD_TYPE%\*.exe" 2>nul

cd ..
echo.
echo [INFO] Build completado en: build\
goto end

:show_help
echo ================================================
echo         AlgoritmiaCombinatoria Builder        
echo               Modo Simple v2.0                
echo ================================================
echo.
echo MODO SIMPLE (como pediste):
echo.
echo   %0 [compilador] [tipo] [cpp_std]
echo.
echo COMPILADORES: gcc, clang, msvc
echo TIPOS: debug, release
echo ESTANDARES C++: 14, 17, 20, 23 (o latest para MSVC)
echo.
echo EJEMPLOS BASICOS:
echo   %0 gcc debug           # GCC Debug C++17 (por defecto)
echo   %0 clang release       # Clang Release C++17
echo   %0 msvc release        # MSVC Release C++17
echo.
echo EJEMPLOS CON VERSION C++:
echo   %0 gcc debug 20        # GCC Debug C++20
echo   %0 clang release 23    # Clang Release C++23
echo   %0 msvc release latest # MSVC Release C++latest
echo   %0 gcc release 14      # GCC Release C++14
echo.
echo SIN ARGUMENTOS:
echo   %0                     # GCC Release C++17 (por defecto)
echo.
echo Para ayuda: %0 -h
goto end

:set_cpp_standard
if "%~1"=="" (
    set "CPP_STANDARD=17"
    goto :eof
)
if "%~1"=="14" (
    set "CPP_STANDARD=14"
    goto :eof
)
if "%~1"=="17" (
    set "CPP_STANDARD=17"
    goto :eof
)
if "%~1"=="20" (
    set "CPP_STANDARD=20"
    goto :eof
)
if "%~1"=="23" (
    if "%COMPILER%"=="msvc" (
        echo [INFO] C++23 para MSVC se mapea a 'latest'
        set "CPP_STANDARD=latest"
    ) else (
        set "CPP_STANDARD=23"
    )
    goto :eof
)
if "%~1"=="latest" (
    if "%COMPILER%"=="msvc" (
        echo [INFO] Usando 'latest' para MSVC (equivale a C++23)
        set "CPP_STANDARD=latest"
        goto :eof
    ) else (
        echo [WARNING] 'latest' solo valido para MSVC, usando C++23
        set "CPP_STANDARD=23"
        goto :eof
    )
)
echo [WARNING] Version C++ '%~1' no valida, usando C++17 por defecto
set "CPP_STANDARD=17"
goto :eof

:end