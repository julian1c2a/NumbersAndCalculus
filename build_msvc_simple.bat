@echo off
setlocal enabledelayedexpansion

echo Testing MSVC build...

rem Verificar argumentos
set "COMPILER=%~1"
set "BUILD_TYPE=%~2"
set "CPP_STANDARD=%~3"

if "%COMPILER%"=="" set "COMPILER=msvc"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=debug"
if "%CPP_STANDARD%"=="" set "CPP_STANDARD=17"

echo Compilador: %COMPILER%
echo Build Type: %BUILD_TYPE%
echo Estandar C++: %CPP_STANDARD%

rem Configurar entorno MSVC
if "%COMPILER%"=="msvc" (
    echo Configurando MSVC...
    
    rem Buscar Visual Studio
    set "VCVARS_PATH="
    if exist "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        set "VCVARS_PATH=D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
        echo Encontrado VS 2022 en D:\Program Files
    )
    
    if "!VCVARS_PATH!"=="" (
        echo ERROR: Visual Studio no encontrado
        exit /b 1
    )
    
    echo Ejecutando vcvars64...
    call "!VCVARS_PATH!" > nul 2>&1
    if errorlevel 1 (
        echo ERROR: Fallo vcvars64
        exit /b 1
    )
    
    echo SUCCESS: Entorno MSVC configurado
    
    rem Limpiar variables problemáticas
    set C_INCLUDE_PATH=
    set CPLUS_INCLUDE_PATH=
    set CPATH=
    set LIBRARY_PATH=
    set LD_LIBRARY_PATH=
    set PKG_CONFIG_PATH=
    
    echo Variables limpiadas
)

rem Limpiar build anterior
if exist build (
    echo Limpiando build...
    rmdir /s /q build 2>nul
)

rem Crear directorio build
mkdir build
cd build

rem Configurar CMake para MSVC usando NMake generator
set CMAKE_ARGS=-G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD%

if "%COMPILER%"=="msvc" (
    echo Configurando para MSVC con Visual Studio generator
)

rem Verificar herramientas
cl >nul 2>&1
if errorlevel 1 (
    echo ERROR: cl.exe no encontrado
    cd ..
    exit /b 1
)

cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: cmake no encontrado
    cd ..
    exit /b 1
)

echo Ejecutando CMake...
cmake .. %CMAKE_ARGS%
if errorlevel 1 (
    echo ERROR: Error en configuracion
    cd ..
    exit /b 1
)

echo Construyendo...
nmake
if errorlevel 1 (
    echo ERROR: Error en construccion
    cd ..
    exit /b 1
)

echo SUCCESS: Build completado
cd ..