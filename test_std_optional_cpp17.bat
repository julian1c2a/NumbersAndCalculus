@echo off
echo === Compilando con C++17 para habilitar std::optional ===
echo.

echo Compilando test_optional_cpp17_demo.cpp...
"C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -Wall -Wextra -O2 test_optional_cpp17_demo.cpp -o test_optional_cpp17_demo.exe
if %errorlevel% equ 0 (
    echo ✓ Compilación exitosa
    echo.
    echo Ejecutando demostración...
    echo.
    test_optional_cpp17_demo.exe
) else (
    echo ✗ Error de compilación
    exit /b 1
)

echo.
echo === Compilando test simple de std::optional ===
echo.
"C:\msys64\ucrt64\bin\g++.exe" -std=c++17 test_optional_simple.cpp -o test_optional_simple.exe
if %errorlevel% equ 0 (
    echo ✓ Compilación de test simple exitosa
    echo.
    echo Ejecutando test simple...
    echo.
    test_optional_simple.exe
) else (
    echo ✗ Error en test simple
)

echo.
echo === Verificando versión C++ ===
echo.
"C:\msys64\ucrt64\bin\g++.exe" -std=c++17 test_cpp_version.cpp -o test_cpp_version.exe
if %errorlevel% equ 0 (
    echo ✓ Test de versión compilado
    echo.
    test_cpp_version.exe
) else (
    echo ✗ Error en test de versión
)

pause