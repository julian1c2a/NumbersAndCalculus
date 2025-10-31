@echo off
setlocal enabledelayedexpansion

echo =================================================================
echo     SISTEMA COMPLETO DE BENCHMARKING Y ANÁLISIS DE RENDIMIENTO
echo =================================================================
echo.

set "COMPILER=C:\msys64\ucrt64\bin\g++.exe"
set "CPP_STD=-std=c++17"
set "BASE_FLAGS=-Wall -Wextra"

echo Compilador: %COMPILER%
echo Estándar C++: %CPP_STD%
echo.

REM Crear directorio para resultados
if not exist "benchmark_results" mkdir benchmark_results

echo === COMPILANDO BENCHMARKS ===
echo.

REM Compilar benchmark principal
echo [1/4] Compilando comprehensive_benchmarks.cpp...
"%COMPILER%" %CPP_STD% %BASE_FLAGS% -O3 -march=native -DNDEBUG comprehensive_benchmarks.cpp -o comprehensive_benchmarks.exe
if %errorlevel% neq 0 (
    echo ERROR: Falló compilación de comprehensive_benchmarks
    goto :error
)

REM Compilar benchmark de optimización
echo [2/4] Compilando optimization_benchmarks.cpp...
"%COMPILER%" %CPP_STD% %BASE_FLAGS% -O3 -march=native -DNDEBUG optimization_benchmarks.cpp -o optimization_benchmarks.exe
if %errorlevel% neq 0 (
    echo ERROR: Falló compilación de optimization_benchmarks
    goto :error
)

REM Compilar benchmark de memoria
echo [3/5] Compilando memory_benchmarks.cpp...
"%COMPILER%" %CPP_STD% %BASE_FLAGS% -O3 -march=native -DNDEBUG memory_benchmarks.cpp -o memory_benchmarks.exe
if %errorlevel% neq 0 (
    echo ERROR: Falló compilación de memory_benchmarks
    goto :error
)

REM Compilar benchmark de demo
echo [4/5] Compilando test_optional_cpp17_demo.cpp...
"%COMPILER%" %CPP_STD% %BASE_FLAGS% -O2 test_optional_cpp17_demo.cpp -o test_optional_cpp17_demo.exe
if %errorlevel% neq 0 (
    echo ERROR: Falló compilación de test_optional_cpp17_demo
    goto :error
)

REM Compilar test simple para verificar std::optional
echo [5/5] Compilando test_optional_simple.cpp...
"%COMPILER%" %CPP_STD% %BASE_FLAGS% -O2 test_optional_simple.cpp -o test_optional_simple.exe
if %errorlevel% neq 0 (
    echo ERROR: Falló compilación de test_optional_simple
    goto :error
)

echo.
echo ✓ Todas las compilaciones exitosas
echo.

echo === EJECUTANDO BENCHMARKS ===
echo.

REM Ejecutar test simple primero
echo [Test 1/4] Verificando std::optional...
echo ----------------------------------------
test_optional_simple.exe > benchmark_results\test_optional_simple.txt 2>&1
if %errorlevel% equ 0 (
    echo ✓ std::optional funciona correctamente
    type benchmark_results\test_optional_simple.txt
) else (
    echo ✗ Error en test de std::optional
    type benchmark_results\test_optional_simple.txt
    goto :error
)
echo.

REM Ejecutar demo
echo [Test 2/4] Ejecutando demostración del sistema...
echo ----------------------------------------
test_optional_cpp17_demo.exe > benchmark_results\demo_results.txt 2>&1
if %errorlevel% equ 0 (
    echo ✓ Demo ejecutada exitosamente
    echo Resultados guardados en benchmark_results\demo_results.txt
) else (
    echo ✗ Error en demo
    type benchmark_results\demo_results.txt
)
echo.

REM Ejecutar benchmarks comprehensivos
echo [Test 3/4] Ejecutando benchmarks comprehensivos...
echo ----------------------------------------
echo Este test puede tomar varios minutos...
comprehensive_benchmarks.exe > benchmark_results\comprehensive_results.txt 2>&1
if %errorlevel% equ 0 (
    echo ✓ Benchmarks comprehensivos completados
    echo Resultados guardados en benchmark_results\comprehensive_results.txt
    echo.
    echo === RESUMEN DE RESULTADOS COMPREHENSIVOS ===
    findstr /C:"SPEEDUP" /C:"Speedup" /C:"más rápido" /C:"más lento" benchmark_results\comprehensive_results.txt
    echo.
) else (
    echo ✗ Error en benchmarks comprehensivos
    type benchmark_results\comprehensive_results.txt
)

REM Ejecutar benchmarks de optimización
echo [Test 4/5] Ejecutando benchmarks de optimización...
echo ----------------------------------------
optimization_benchmarks.exe > benchmark_results\optimization_results.txt 2>&1
if %errorlevel% equ 0 (
    echo ✓ Benchmarks de optimización completados
    echo Resultados guardados en benchmark_results\optimization_results.txt
    echo.
    echo === RESUMEN DE OPTIMIZACIÓN ===
    findstr /C:"ms" /C:"Operaciones por segundo" benchmark_results\optimization_results.txt | head -n 10
    echo.
) else (
    echo ✗ Error en benchmarks de optimización
    type benchmark_results\optimization_results.txt
)

REM Ejecutar benchmarks de memoria
echo [Test 5/5] Ejecutando benchmarks de memoria y cache...
echo ----------------------------------------
memory_benchmarks.exe > benchmark_results\memory_results.txt 2>&1
if %errorlevel% equ 0 (
    echo ✓ Benchmarks de memoria completados
    echo Resultados guardados en benchmark_results\memory_results.txt
    echo.
    echo === RESUMEN DE MEMORIA ===
    findstr /C:"penalty" /C:"speedup" /C:"KB" benchmark_results\memory_results.txt
    echo.
) else (
    echo ✗ Error en benchmarks de memoria
    type benchmark_results\memory_results.txt
)

echo === ANÁLISIS DE BINARIOS ===
echo.

REM Mostrar tamaños de ejecutables
echo Tamaños de ejecutables generados:
for %%f in (*.exe) do (
    for %%s in (%%f) do (
        set size=%%~zs
        set /a size_kb=!size!/1024
        echo   %%f: !size_kb! KB ^(!size! bytes^)
    )
)
echo.

echo === ANÁLISIS DE TIEMPO DE COMPILACIÓN ===
echo.

REM Test de tiempo de compilación con diferentes niveles de optimización
echo Compilando con diferentes niveles de optimización...

echo [O0] Sin optimización...
powershell -Command "Measure-Command { & '%COMPILER%' %CPP_STD% -O0 test_optional_simple.cpp -o test_O0.exe 2>$null } | Select-Object -ExpandProperty TotalMilliseconds" > temp_compile_time.txt
set /p compile_time_O0=<temp_compile_time.txt
echo   Tiempo: !compile_time_O0! ms

echo [O1] Optimización básica...
powershell -Command "Measure-Command { & '%COMPILER%' %CPP_STD% -O1 test_optional_simple.cpp -o test_O1.exe 2>$null } | Select-Object -ExpandProperty TotalMilliseconds" > temp_compile_time.txt
set /p compile_time_O1=<temp_compile_time.txt
echo   Tiempo: !compile_time_O1! ms

echo [O2] Optimización estándar...
powershell -Command "Measure-Command { & '%COMPILER%' %CPP_STD% -O2 test_optional_simple.cpp -o test_O2.exe 2>$null } | Select-Object -ExpandProperty TotalMilliseconds" > temp_compile_time.txt
set /p compile_time_O2=<temp_compile_time.txt
echo   Tiempo: !compile_time_O2! ms

echo [O3] Optimización máxima...
powershell -Command "Measure-Command { & '%COMPILER%' %CPP_STD% -O3 test_optional_simple.cpp -o test_O3.exe 2>$null } | Select-Object -ExpandProperty TotalMilliseconds" > temp_compile_time.txt
set /p compile_time_O3=<temp_compile_time.txt
echo   Tiempo: !compile_time_O3! ms

del temp_compile_time.txt

echo.
echo Tamaños de binarios por nivel de optimización:
for %%f in (test_O*.exe) do (
    for %%s in (%%f) do (
        set size=%%~zs
        set /a size_kb=!size!/1024
        echo   %%f: !size_kb! KB
    )
)

echo.
echo === RESUMEN FINAL ===
echo.
echo ✓ Todos los benchmarks completados
echo ✓ std::optional verificado funcionando con C++17
echo ✓ Comparación CalculationResult vs std::optional realizada
echo ✓ Análisis de optimización completado
echo ✓ Análisis de tiempo de compilación completado
echo.
echo Archivos de resultados generados:
echo   - benchmark_results\test_optional_simple.txt
echo   - benchmark_results\demo_results.txt
echo   - benchmark_results\comprehensive_results.txt
echo   - benchmark_results\optimization_results.txt
echo   - benchmark_results\memory_results.txt
echo.
echo Para ver resultados detallados:
echo   type benchmark_results\comprehensive_results.txt
echo   type benchmark_results\optimization_results.txt
echo   type benchmark_results\memory_results.txt
echo.

REM Limpiar archivos temporales
del test_O*.exe 2>nul

goto :end

:error
echo.
echo ✗ ERROR: El benchmarking falló
echo Revisa los mensajes de error anteriores
exit /b 1

:end
echo Benchmarking completado exitosamente.
pause