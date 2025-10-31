#!/bin/bash

# Script para corregir caracteres UTF-8 restantes que causan problemas en terminal

FILES_TO_FIX=(
    "comprehensive_benchmarks.cpp"
    "include/number_calc/comprehensive_benchmarks.hpp"
)

for file in "${FILES_TO_FIX[@]}"; do
    if [ -f "$file" ]; then
        echo "Procesando: $file"
        
        # Usar perl para hacer reemplazos más robustos
        perl -i -pe '
            s/├│/o/g;
            s/├¡/i/g;
            s/├®/e/g;
            s/├▒/n/g;
            s/Tama├▒os/Tamanos/g;
            s/Alineaci├│n/Alineacion/g;
            s/Creaci├│n/Creacion/g;
            s/Iteraci├│n/Iteracion/g;
            s/PREDICCI├ôN/PREDICCION/g;
            s/├¡ndices/indices/g;
            s/An├ílisis/Analisis/g;
            s/operaci├│n/operacion/g;
            s/comparaci├│n/comparacion/g;
            s/divisi├│n/division/g;
        ' "$file"
        
        echo "  -> Archivo procesado"
    else
        echo "  -> Archivo no encontrado: $file"
    fi
done

echo "=== CORRECCION UTF-8 ADICIONAL COMPLETADA ==="