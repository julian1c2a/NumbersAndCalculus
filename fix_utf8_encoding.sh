#!/bin/bash

# Script para corregir caracteres UTF-8 problemáticos en archivos de código
# Este script reemplaza caracteres especiales con equivalentes ASCII

echo "=== CORRECTOR DE CODIFICACION UTF-8 ==="
echo "Corrigiendo caracteres problematicos en archivos de codigo..."

# Definir los reemplazos más comunes
declare -A replacements=(
    ["á"]="a"
    ["é"]="e" 
    ["í"]="i"
    ["ó"]="o"
    ["ú"]="u"
    ["ñ"]="n"
    ["Á"]="A"
    ["É"]="E"
    ["Í"]="I"
    ["Ó"]="O"
    ["Ú"]="U"
    ["Ñ"]="N"
    ["Ô£ô"]="-->"
    ["Ô£à"]="-->"
    ["├®"]="e"
    ["├│"]="o"
    ["├¡"]="i"
    ["├ü"]="a"
    ["├╡"]="u"
    ["├¿"]="n"
    ["DIVISIÓN"]="DIVISION"
    ["INFORMACIÓN"]="INFORMACION"
    ["ANÁLISIS"]="ANALISIS"
    ["ITERACIÓN"]="ITERACION"
    ["OPTIMIZACIÓN"]="OPTIMIZACION"
    ["COMPILACIÓN"]="COMPILACION"
    ["CONFIGURACIÓN"]="CONFIGURACION"
    ["FUNCIÓN"]="FUNCION"
    ["DESCRIPCIÓN"]="DESCRIPCION"
    ["CONCLUSIÓN"]="CONCLUSION"
    ["más"]="mas"
    ["también"]="tambien"
    ["iteración"]="iteracion"
    ["optimización"]="optimizacion"
    ["función"]="funcion"
    ["compilación"]="compilacion"
    ["configuración"]="configuracion"
    ["descripción"]="descripcion"
    ["conclusión"]="conclusion"
    ["información"]="informacion"
    ["análisis"]="analisis"
    ["división"]="division"
    ["rápido"]="rapido"
    ["lento"]="lento"
    ["número"]="numero"
    ["índice"]="indice"
    ["índices"]="indices"
    ["creación"]="creacion"
    ["alineación"]="alineacion"
    ["precisión"]="precision"
    ["expresión"]="expresion"
    ["ejecución"]="ejecucion"
    ["medición"]="medicion"
    ["comparación"]="comparacion"
    ["verificación"]="verificacion"
    ["implementación"]="implementacion"
    ["declaración"]="declaracion"
    ["especialización"]="especializacion"
    ["pequeña"]="pequena"
    ["pequeño"]="pequeno"
    ["válido"]="valido"
    ["útil"]="util"
    ["específico"]="especifico"
    ["específicos"]="especificos"
    ["específicas"]="especificas"
    ["específica"]="especifica"
)

# Función para aplicar reemplazos a un archivo
fix_file() {
    local file="$1"
    local temp_file="${file}.tmp"
    
    echo "Procesando: $file"
    
    # Crear copia del archivo original
    cp "$file" "$temp_file"
    
    # Aplicar cada reemplazo
    for search in "${!replacements[@]}"; do
        replace="${replacements[$search]}"
        sed -i "s/$search/$replace/g" "$temp_file" 2>/dev/null || true
    done
    
    # Solo reemplazar si hubo cambios
    if ! cmp -s "$file" "$temp_file"; then
        mv "$temp_file" "$file"
        echo "  -> Archivo corregido"
    else
        rm "$temp_file"
        echo "  -> Sin cambios necesarios"
    fi
}

# Procesar archivos de código
echo -e "\nProcesando archivos .cpp .hpp .h .cc .cxx..."
find . -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.cc" -o -name "*.cxx" \) | while read -r file; do
    fix_file "$file"
done

echo -e "\nProcesando archivos de documentación..."
find . -type f \( -name "*.md" -o -name "*.txt" \) | while read -r file; do
    fix_file "$file"
done

echo -e "\n=== CORRECCION COMPLETADA ==="
echo "Todos los archivos han sido procesados."
echo "Los caracteres especiales han sido reemplazados con equivalentes ASCII."