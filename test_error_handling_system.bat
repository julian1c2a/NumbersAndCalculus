@echo off
echo ================================================
echo COMPILANDO Y EJECUTANDO SISTEMA DE MANEJO DE ERRORES
echo ================================================

cd /d "c:\msys64\ucrt64\home\julian\CppProjects\AlgoritmiaCombinatoria"

echo.
echo Compilando test del nuevo sistema...
"c:\msys64\ucrt64\bin\g++.exe" -std=c++14 -Wall -Wextra -O2 test_noexcept_optional_system.cpp -o test_noexcept_optional_system.exe

if %ERRORLEVEL% EQU 0 (
    echo ✅ Compilacion exitosa!
    echo.
    echo Ejecutando test del sistema de manejo de errores...
    echo ================================================
    test_noexcept_optional_system.exe
    echo ================================================
    echo Test completado!
) else (
    echo ❌ Error en la compilacion.
)

echo.
pause