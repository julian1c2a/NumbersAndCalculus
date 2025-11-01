@echo off
setlocal enabledelayedexpansion

rem Script de construccion con directorios separados - AlgoritmiaCombinatoria
rem Uso: build_multi_final.bat [compilador] [tipo] [estandar]  
rem Crea: build_[compilador]_[tipo]_cpp[estandar]\ para cada configuracion
rem Soporte para: gcc, clang, msvc

rem Verificar si es solicitud de ayuda
if "%~1"=="-h" goto show_help
if "%~1"=="--help" goto show_help

rem Configurar valores por defecto
set "COMPILER=%~1"
set "BUILD_TYPE=%~2"
set "CPP_STANDARD=%~3"

rem Validar argumentos
if "%COMPILER%"=="" set "COMPILER=gcc"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=Release"
if "%CPP_STANDARD%"=="" set "CPP_STANDARD=17"

rem Convertir a formato CMake
if /i "%BUILD_TYPE%"=="debug" set "BUILD_TYPE=Debug"
if /i "%BUILD_TYPE%"=="release" set "BUILD_TYPE=Release"

rem Crear nombre del directorio específico
for %%i in ("%COMPILER%") do set "COMPILER_LOWER=%%~i"
for %%i in ("%BUILD_TYPE%") do set "BUILD_TYPE_LOWER=%%~i"
call :tolower COMPILER_LOWER
call :tolower BUILD_TYPE_LOWER
set "BUILD_DIR=build_%COMPILER_LOWER%_%BUILD_TYPE_LOWER%_cpp%CPP_STANDARD%"

rem Mostrar configuracion
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

rem Limpiar directorio específico anterior
if exist "%BUILD_DIR%" (
    echo [INFO] Limpiando %BUILD_DIR% anterior...
    rmdir /s /q "%BUILD_DIR%"
)

rem Crear directorio específico
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

rem Configurar PATH para herramientas
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

rem Configurar argumentos de CMake
set "CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD%"

rem Configurar compilador especifico
if /i "%COMPILER%"=="gcc" (
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe"
    echo [INFO] Configurando para GCC con C++%CPP_STANDARD%
)

if /i "%COMPILER%"=="clang" (
    set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/clang.exe"
    echo [INFO] Configurando para Clang con C++%CPP_STANDARD%
)

if /i "%COMPILER%"=="msvc" (
    echo [INFO] Configurando para MSVC con C++%CPP_STANDARD%
    
    rem Configurar entorno MSVC
    if "%VCINSTALLDIR%"=="" (
        echo [INFO] Configurando entorno MSVC...
        
        set "VCVARS_PATH="
        if exist "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
            set "VCVARS_PATH=D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
        )
        if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
            set "VCVARS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
        )
        
        if "!VCVARS_PATH!"=="" (
            echo [ERROR] Visual Studio no encontrado
            cd ..
            exit /b 1
        )
        
        echo [INFO] Ejecutando vcvars64...
        call "!VCVARS_PATH!" > nul 2>&1
        if errorlevel 1 (
            echo [ERROR] Error al configurar MSVC
            cd ..
            exit /b 1
        )
        
        echo [SUCCESS] Entorno MSVC configurado
        
        rem Limpiar variables problemáticas
        set C_INCLUDE_PATH=
        set CPLUS_INCLUDE_PATH=
        set CPATH=
        set LIBRARY_PATH=
        set LD_LIBRARY_PATH=
        set PKG_CONFIG_PATH=
    )
    
    rem Configurar CMake para MSVC
    set CMAKE_ARGS=-G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD%
    
    rem Verificar vcpkg
    if exist "C:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
        set "CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
        echo [INFO] Usando vcpkg toolchain
    ) else (
        echo [WARNING] vcpkg no encontrado - Boost sera opcional
    )
)

rem Ejecutar CMake
echo [INFO] Configurando proyecto...
cmake .. %CMAKE_ARGS%
if errorlevel 1 (
    echo [ERROR] Error en la configuracion
    cd ..
    exit /b 1
)

rem Construir
echo [INFO] Construyendo...
cmake --build . --config %BUILD_TYPE% --parallel 4

if errorlevel 1 (
    echo [ERROR] Error en la construccion
    cd ..
    exit /b 1
)

echo.
echo echo [SUCCESS] Proceso completado exitosamente!
exit /b 0

:tolower
set "str=!%1!"
for %%i in ("A=a" "B=b" "C=c" "D=d" "E=e" "F=f" "G=g" "H=h" "I=i" "J=j" "K=k" "L=l" "M=m" "N=n" "O=o" "P=p" "Q=q" "R=r" "S=s" "T=t" "U=u" "V=v" "W=w" "X=x" "Y=y" "Z=z") do (
    call set "str=%%str:%%~i%%"
)
set "%1=!str!"
exit /b 0
echo.

rem Mostrar ejecutables
echo Ejecutables generados:
if exist "*.exe" dir /b *.exe 2>nul
if exist "%BUILD_TYPE%\" if exist "%BUILD_TYPE%\*.exe" dir /b "%BUILD_TYPE%\*.exe" 2>nul

cd ..
echo.
echo [INFO] Build completado en: %BUILD_DIR%\

echo.
echo ================================================
echo            BUILD COMPLETADO EXITOSAMENTE        
echo ================================================
echo.
echo Configuracion: %COMPILER% %BUILD_TYPE% C++%CPP_STANDARD%
echo Directorio: %BUILD_DIR%\
echo.
echo Directorios de build existentes:
for /d %%d in (build_*) do echo   %%d\
echo.
echo [INFO] Cambiar al directorio: cd %BUILD_DIR%
echo [INFO] Limpiar todos los builds: for /d %%d in (build_*) do rmdir /s /q "%%d"
goto end

:show_help
echo.
echo ================================================
echo         AlgoritmiaCombinatoria Builder        
echo              Modo Simple v2.1                
echo ================================================
echo.
echo SINTAXIS: %0 [compilador] [tipo] [estandar]
echo.
echo COMPILADORES: gcc, clang, msvc
echo TIPOS: debug, release
echo ESTANDARES C++: 14, 17, 20, 23
echo.
echo EJEMPLOS:
echo   %0 gcc debug 17      # GCC Debug C++17
echo   %0 clang release 20  # Clang Release C++20
echo   %0 msvc release 23   # MSVC Release C++23
echo   %0                   # GCC Release C++17 (por defecto)
echo.
goto end

:end