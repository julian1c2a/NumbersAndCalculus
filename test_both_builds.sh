#!/bin/bash

echo "=== COMPARACIÓN DE SCRIPTS BUILD ==="
echo ""

echo "1. Probando build_simple.sh (ACTUALIZADO)"
echo "Comando: ./build_simple.sh gcc debug 17"
echo "---"
./build_simple.sh gcc debug 17
echo ""

echo "2. Probando build_simple.bat"  
echo "Comando: ./build_simple.bat gcc debug 17"
echo "---"
./build_simple.bat gcc debug 17
echo ""

echo "=== COMPARACIÓN COMPLETADA ==="