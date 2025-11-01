@echo off
setlocal enabledelayedexpansion

rem Script de construccion con directorios separados por configuracion
rem Uso: build_multi_dirs.bat [compilador] [tipo] [estandar]
rem Crea: build_[compilador]_[tipo]_cpp[estandar]\

rem Verificar ayuda
if "%~1"=="-h" goto show_help
if "%~1"=="--help" goto show_help
if "%~1"=="/?" goto show_help

rem Configurar valores por defecto
set "COMPILER=%~1"
set "BUILD_TYPE=%~2"
set "CPP_STANDARD=%~3"

if "%COMPILER%"=="" set "COMPILER=gcc"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=Release"
if "%CPP_STANDARD%"=="" set "CPP_STANDARD=17"

rem Normalizar build type
if /I "%BUILD_TYPE%"=="debug" set "BUILD_TYPE=Debug"
if /I "%BUILD_TYPE%"=="d" set "BUILD_TYPE=Debug"
if /I "%BUILD_TYPE%"=="release" set "BUILD_TYPE=Release"
if /I "%BUILD_TYPE%"=="r" set "BUILD_TYPE=Release"

rem Crear nombre del directorio específico
for %%i in ("%COMPILER%") do set "COMPILER_LOWER=%%~i"
for %%i in ("%BUILD_TYPE%") do set "BUILD_TYPE_LOWER=%%~i"
call :tolower COMPILER_LOWER
call :tolower BUILD_TYPE_LOWER

set "BUILD_DIR=build_%COMPILER_LOWER%_%BUILD_TYPE_LOWER%_cpp%CPP_STANDARD%"

echo.
echo ================================================
echo     AlgoritmiaCombinatoria Multi-Build System
echo               Directorios Separados
echo ================================================
echo.
echo Compilador: %COMPILER%
echo Build Type: %BUILD_TYPE%
echo Estandar C++: %CPP_STANDARD%
echo Directorio: %BUILD_DIR%
echo.

rem Verificar herramientas
echo [INFO] Verificando herramientas...

rem Verificar CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake no encontrado
    exit /b 1
)
echo [INFO] CMake encontrado

rem Verificar generador segun compilador
if /I "%COMPILER%"=="msvc" (
    set "GENERATOR=Visual Studio 17 2022"
    echo [INFO] Usando Visual Studio Generator para MSVC
) else (
    ninja --version >nul 2>&1
    if errorlevel 1 (
        echo [WARNING] Ninja no encontrado, usando MinGW Makefiles
        set "GENERATOR=MinGW Makefiles"
    ) else (
        echo [INFO] Ninja encontrado
        set "GENERATOR=Ninja"
    )
)

rem Configurar segun compilador
if /I "%COMPILER%"=="gcc" goto config_gcc
if /I "%COMPILER%"=="clang" goto config_clang
if /I "%COMPILER%"=="msvc" goto config_msvc
echo [ERROR] Compilador '%COMPILER%' no soportado
exit /b 1

:config_gcc
echo [INFO] Configurando para GCC con C++%CPP_STANDARD%
set "CMAKE_ARGS=-G "%GENERATOR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD% -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc"
goto build

:config_clang
echo [INFO] Configurando para Clang con C++%CPP_STANDARD%
set "CMAKE_ARGS=-G "%GENERATOR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD% -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
goto build

:config_msvc
echo [INFO] Configurando para MSVC con C++%CPP_STANDARD%

rem Detectar y configurar Visual Studio
call :setup_msvc_env
if errorlevel 1 exit /b 1

set "CMAKE_ARGS=-G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=%CPP_STANDARD%"
goto build

:build
rem Limpiar directorio específico
if exist "%BUILD_DIR%" (
    echo [INFO] Limpiando %BUILD_DIR% anterior...
    rmdir /s /q "%BUILD_DIR%"
)

rem Crear directorio específico
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

rem Para GCC/Clang, necesitamos asegurar que estamos en entorno MSYS2
if /I "%COMPILER%"=="gcc" (
    echo [INFO] Configurando entorno MSYS2 para GCC...
    set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
)
if /I "%COMPILER%"=="clang" (
    echo [INFO] Configurando entorno MSYS2 para Clang...
    set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
)

rem Configurar CMake
echo [INFO] Configurando proyecto en %BUILD_DIR%...
cmake .. %CMAKE_ARGS%
if errorlevel 1 (
    echo [ERROR] Error en la configuracion de CMake
    cd ..
    exit /b 1
)

rem Construir
echo [INFO] Construyendo en %BUILD_DIR%...
cmake --build . --config %BUILD_TYPE%
if errorlevel 1 (
    echo [ERROR] Error en la construccion
    cd ..
    exit /b 1
)

echo.
echo [SUCCESS] Construccion completada en %BUILD_DIR%!
echo.

rem Mostrar ejecutables
echo Ejecutables en %BUILD_DIR%:
if exist "*.exe" (
    dir /b *.exe 2>nul
) else (
    echo Ninguno encontrado
)

cd ..

rem Mostrar resumen
echo.
echo ================================================
echo            BUILD COMPLETADO EXITOSAMENTE
echo ================================================
echo.
echo Configuracion: %COMPILER% %BUILD_TYPE% C++%CPP_STANDARD%
echo Directorio: %BUILD_DIR%
echo.
echo Directorios de build existentes:
for /d %%d in (build_*) do echo   %%d\
echo.
echo [INFO] Puedes cambiar al directorio con: cd %BUILD_DIR%
echo [INFO] Para limpiar todos los builds: rmdir /s /q build_*
goto end

:setup_msvc_env
rem Buscar y configurar Visual Studio 2022
set "VS_PATH="
for %%p in (
    "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    "D:\Program Files\Microsoft Visual Studio\2022\community\VC\Auxiliary\Build\vcvars64.bat"
) do (
    if exist "%%~p" (
        set "VS_PATH=%%~p"
        goto found_vs
    )
)

echo [ERROR] Visual Studio 2022 no encontrado
exit /b 1

:found_vs
echo [INFO] Configurando entorno Visual Studio...
call "%VS_PATH%" >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Error configurando Visual Studio
    exit /b 1
)
echo [INFO] Entorno MSVC configurado correctamente
exit /b 0

:tolower
set "str=!%1!"
for %%i in ("A=a" "B=b" "C=c" "D=d" "E=e" "F=f" "G=g" "H=h" "I=i" "J=j" "K=k" "L=l" "M=m" "N=n" "O=o" "P=p" "Q=q" "R=r" "S=s" "T=t" "U=u" "V=v" "W=w" "X=x" "Y=y" "Z=z") do (
    call set "str=%%str:%%~i%%"
)
set "%1=!str!"
exit /b 0

:show_help
echo.
echo ================================================
echo     AlgoritmiaCombinatoria Multi-Build System
echo               Directorios Separados
echo ================================================
echo.
echo SINTAXIS: %~nx0 [compilador] [tipo] [estandar]
echo.
echo COMPILADORES: gcc, clang, msvc
echo TIPOS: debug, release
echo ESTANDARES C++: 14, 17, 20, 23
echo.
echo DIRECTORIOS DE BUILD:
echo   build_gcc_debug_cpp17\     # GCC Debug C++17
echo   build_clang_release_cpp20\ # Clang Release C++20  
echo   build_msvc_release_cpp23\  # MSVC Release C++23
echo.
echo EJEMPLOS:
echo   %~nx0 gcc debug 17      # -^> build_gcc_debug_cpp17\
echo   %~nx0 clang release 20  # -^> build_clang_release_cpp20\
echo   %~nx0 msvc release 23   # -^> build_msvc_release_cpp23\
echo   %~nx0                   # -^> build_gcc_release_cpp17\
echo.
exit /b 0

:end
echo [SUCCESS] Proceso completado exitosamente!