@echo off
echo Compilando prueba simple de constexpr...

cd /d "c:\msys64\ucrt64\home\julian\CppProjects\AlgoritmiaCombinatoria"

"c:\msys64\ucrt64\bin\g++.exe" -std=c++14 -Wall -Wextra -O2 simple_test_constexpr.cpp -o simple_test_constexpr.exe

if %ERRORLEVEL% EQU 0 (
    echo Compilacion exitosa! Ejecutando...
    echo.
    simple_test_constexpr.exe
) else (
    echo Error en la compilacion.
)

pause