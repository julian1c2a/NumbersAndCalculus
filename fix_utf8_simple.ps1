# Script simple para arreglar UTF-8
$file = "test_small_type_optimizations.cpp"

if (Test-Path $file) {
    Write-Host "Procesando: $file"
    
    # Crear backup
    Copy-Item $file "$file.backup"
    
    # Leer y procesar línea por línea
    $lines = Get-Content $file -Encoding UTF8
    $newLines = @()
    
    foreach ($line in $lines) {
        $newLine = $line
        $newLine = $newLine -replace "✓", "[OK]"
        $newLine = $newLine -replace "🚀", "[ROCKET]"
        $newLine = $newLine -replace "✅", "[CHECK]"
        $newLines += $newLine
    }
    
    # Guardar archivo
    $newLines | Out-File $file -Encoding UTF8
    
    Write-Host "Archivo procesado correctamente"
} else {
    Write-Host "Archivo no encontrado"
}