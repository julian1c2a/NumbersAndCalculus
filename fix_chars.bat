#include "include/utf8_io.hpp"

@echo off
echo Arreglando caracteres UTF-8 en test_small_type_optimizations.cpp

rem Crear backup
copy test_small_type_optimizations.cpp test_small_type_optimizations.cpp.backup

rem Usar PowerShell para hacer reemplazos simples
powershell -Command "(Get-Content 'test_small_type_optimizations.cpp' -Raw) -replace '✓', '[OK]' | Set-Content 'test_small_type_optimizations.cpp' -Encoding UTF8"

powershell -Command "(Get-Content 'test_small_type_optimizations.cpp' -Raw) -replace '🚀', '[ROCKET]' | Set-Content 'test_small_type_optimizations.cpp' -Encoding UTF8"

powershell -Command "(Get-Content 'test_small_type_optimizations.cpp' -Raw) -replace '✅', '[CHECK]' | Set-Content 'test_small_type_optimizations.cpp' -Encoding UTF8"

echo Proceso completado
