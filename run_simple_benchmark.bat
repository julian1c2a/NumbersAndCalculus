@echo off
echo Compilando benchmark simple...

cd /d "c:\msys64\ucrt64\home\julian\CppProjects\AlgoritmiaCombinatoria"

"c:\msys64\ucrt64\bin\g++.exe" -std=c++14 -O3 -DNDEBUG simple_benchmark.cpp -o simple_benchmark.exe

if %ERRORLEVEL% EQU 0 (
    echo ✅ Compilacion exitosa!
    echo.
    echo Ejecutando benchmark...
    echo ================================================
    simple_benchmark.exe
    echo ================================================
) else (
    echo ❌ Error en la compilacion.
)

pause