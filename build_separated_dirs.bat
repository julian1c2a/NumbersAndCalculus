@echo off
setlocal enabledelayedexpansion

rem Script de construccion con directorios separados - Version basada en build_simple_clean.bat
rem Uso: build_separated_dirs.bat [compilador] [tipo] [estandar]

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
set "COMPILER_LOWER=%COMPILER%"
set "BUILD_TYPE_LOWER=%BUILD_TYPE%"
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

rem Limpiar directorio específico si existe
if exist "%BUILD_DIR%" (
    echo [INFO] Limpiando %BUILD_DIR% anterior...
    rmdir /s /q "%BUILD_DIR%"
)

rem Configurar según compilador (lógica del script funcional)
if /I "%COMPILER%"=="gcc" goto config_gcc
if /I "%COMPILER%"=="clang" goto config_clang  
if /I "%COMPILER%"=="msvc" goto config_msvc
echo [ERROR] Compilador '%COMPILER%' no soportado
echo [INFO] Compiladores soportados: gcc, clang, msvc
exit /b 1

:config_gcc
echo [INFO] Configurando para GCC con C++%CPP_STANDARD%
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe ..
goto build_project

:config_clang
echo [INFO] Configurando para Clang con C++%CPP_STANDARD%
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_CXX_STANDARD=%CPP_STANDARD% -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/clang.exe -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe ..
goto build_project

:config_msvc
echo [INFO] Configurando para MSVC con C++%CPP_STANDARD%

rem Configurar entorno MSVC (lógica del script funcional)
call :setup_msvc_env
if errorlevel 1 exit /b 1

mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=%CPP_STANDARD% ..
goto build_project

:build_project
if errorlevel 1 (
    echo [ERROR] Error en la configuracion de CMake
    cd ..
    exit /b 1
)

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
    for %%f in (*.exe) do echo   %%f
) else (
    echo   Ninguno encontrado en directorio raíz
)

rem Buscar en subdirectorios para MSVC
if /I "%COMPILER%"=="msvc" (
    if exist "%BUILD_TYPE%\*.exe" (
        echo   En subdirectorio %BUILD_TYPE%:
        for %%f in (%BUILD_TYPE%\*.exe) do echo     %%f
    )
)

cd ..

rem Mostrar resumen final
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

:setup_msvc_env
echo [INFO] Detectando Visual Studio 2022...

rem Buscar Visual Studio en ubicaciones comunes
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

echo [ERROR] Visual Studio 2022 no encontrado en ubicaciones estandar
echo [HELP] Instalar Visual Studio 2022 Community desde visualstudio.microsoft.com
exit /b 1

:found_vs
echo [INFO] Configurando entorno Visual Studio...
echo [INFO] Ubicacion: %VS_PATH%
call "%VS_PATH%" >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Error al configurar el entorno Visual Studio
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
echo DIRECTORIOS GENERADOS:
echo   build_gcc_debug_cpp17\     # GCC Debug C++17
echo   build_clang_release_cpp20\ # Clang Release C++20
echo   build_msvc_release_cpp23\  # MSVC Release C++23
echo.
echo EJEMPLOS:
echo   %~nx0 gcc debug 17      # Crea build_gcc_debug_cpp17\
echo   %~nx0 clang release 20  # Crea build_clang_release_cpp20\
echo   %~nx0 msvc release 23   # Crea build_msvc_release_cpp23\
echo   %~nx0                   # Crea build_gcc_release_cpp17\
echo.
echo GESTION DE DIRECTORIOS:
echo   - Cada configuracion usa su propio directorio
echo   - Los directorios se limpian automaticamente antes del build
echo   - Puedes mantener multiples configuraciones simultaneamente
echo.
exit /b 0

:end
echo [SUCCESS] Proceso completado exitosamente!