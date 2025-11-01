#!/bin/bash

# Test del script arreglado desde PowerShell
# Este script se ejecutará en MSYS2 bash para probar la corrección

echo "================================================"
echo "  PRUEBA: Script Bash Arreglado (build_simple.sh)"
echo "================================================"
echo ""

# Mostrar información del entorno
echo "Entorno actual:"
echo "- Shell: $0"
echo "- Sistema: $(uname -s)"
echo "- PWD: $(pwd)"
echo ""

# Probar el script arreglado con msvc debug 17
echo "Ejecutando: ./build_simple.sh msvc debug 17"
echo "Iniciando en 3 segundos..."
sleep 1
echo "3..."
sleep 1  
echo "2..."
sleep 1
echo "1..."
echo ""

# Ejecutar el script con timeout para evitar cuelgues
timeout 30s ./build_simple.sh msvc debug 17

RESULT=$?

echo ""
echo "================================================"
echo "  RESULTADO DE LA PRUEBA"
echo "================================================"

if [ $RESULT -eq 0 ]; then
    echo "✅ ÉXITO: Script completado correctamente"
elif [ $RESULT -eq 124 ]; then
    echo "❌ TIMEOUT: Script se colgó después de 30 segundos"
else
    echo "⚠️  WARNING: Script terminó con código $RESULT"
fi

echo ""
echo "Fin de la prueba."