@echo off
echo ================================================
echo COMPILANDO Y EJECUTANDO BENCHMARK DE TABLAS CONSTEXPR
echo ================================================

cd /d "c:\msys64\ucrt64\home\julian\CppProjects\AlgoritmiaCombinatoria"

echo.
echo Compilando con optimizaciones O3...
"c:\msys64\ucrt64\bin\g++.exe" -std=c++14 -Wall -Wextra -O3 -DNDEBUG benchmark_constexpr_tables.cpp -o benchmark_constexpr_tables.exe

if %ERRORLEVEL% EQU 0 (
    echo ✅ Compilacion exitosa!
    echo.
    echo Ejecutando benchmark...
    echo ================================================
    benchmark_constexpr_tables.exe
    echo ================================================
    echo Benchmark completado!
) else (
    echo ❌ Error en la compilacion.
    echo.
    echo Intentando compilacion con informacion de debug...
    "c:\msys64\ucrt64\bin\g++.exe" -std=c++14 -Wall -Wextra -O1 -v benchmark_constexpr_tables.cpp -o benchmark_constexpr_tables.exe 2>&1
)

echo.
pause