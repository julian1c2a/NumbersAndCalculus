# Script PowerShell para arreglar caracteres UTF-8 problemáticos
# Reemplaza caracteres problemáticos con equivalentes ASCII

$files = @(
    "test_small_type_optimizations.cpp"
)

Write-Host "=== Arreglando caracteres UTF-8 problemáticos ==="

foreach ($file in $files) {
    if (Test-Path $file) {
        Write-Host "Procesando: $file"
        
        # Crear backup
        Copy-Item $file "$file.backup"
        
        # Leer contenido completo
        $content = Get-Content $file -Raw -Encoding UTF8
        
        # Reemplazos específicos de caracteres problemáticos
        $content = $content -replace "✓", "[OK]"
        $content = $content -replace "✗", "[ERROR]"
        $content = $content -replace "función", "funcion"
        $content = $content -replace "genérica", "generica"
        $content = $content -replace "causaría", "causaria"
        $content = $content -replace "ó", "o"
        $content = $content -replace "á", "a"
        $content = $content -replace "é", "e"
        $content = $content -replace "í", "i"
        $content = $content -replace "ú", "u"
        $content = $content -replace "ñ", "n"
        $content = $content -replace "Ñ", "N"
        $content = $content -replace "ü", "u"
        $content = $content -replace "Ü", "U"
        $content = $content -replace "¿", ""
        $content = $content -replace "¡", ""
        $content = $content -replace "–", "-"
        $content = $content -replace "—", "-"
        $content = $content -replace """, '"'
        $content = $content -replace """, '"'
        $content = $content -replace "'", "'"
        $content = $content -replace "'", "'"
        $content = $content -replace "…", "..."
        $content = $content -replace "•", "*"
        $content = $content -replace "←", "<-"
        $content = $content -replace "→", "->"
        $content = $content -replace "↑", "^"
        $content = $content -replace "↓", "v"
        $content = $content -replace "°", "deg"
        $content = $content -replace "±", "+/-"
        $content = $content -replace "×", "x"
        $content = $content -replace "÷", "/"
        $content = $content -replace "≤", "<="
        $content = $content -replace "≥", ">="
        $content = $content -replace "≠", "!="
        $content = $content -replace "≈", "~="
        $content = $content -replace "∞", "INF"
        $content = $content -replace "π", "PI"
        $content = $content -replace "🚀", "[ROCKET]"
        $content = $content -replace "✅", "[CHECK]"
        
        # Guardar con encoding ASCII seguro
        $content | Out-File $file -Encoding UTF8
        
        Write-Host "  -> Caracteres arreglados en $file"
    } else {
        Write-Host "  -> Archivo no encontrado: $file"
    }
}

Write-Host ""
Write-Host "=== Procesamiento completo ==="
Write-Host "Los archivos originales se guardaron con extensión .backup"