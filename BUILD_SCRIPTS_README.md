# AlgoritmiaCombinatoria - Scripts de Construccion Multiplataforma

Este proyecto incluye scripts de construccion equivalentes para Windows y Unix/Linux que automatizan completamente la compilacion con CMake y Ninja.

## Scripts Disponibles

### 1. `build_simple_clean.bat` (Windows Native)
Script principal para Windows que funciona desde PowerShell, CMD y MSYS2.

**Sintaxis:** `¿Usando CMake y ninja no se puede arreglar?`
```batch
.\build_simple_clean.bat [compilador] [tipo] [estandar]
```

**Ejemplos:**
```batch
.\build_simple_clean.bat gcc debug 17      # GCC Debug C++17
.\build_simple_clean.bat clang release 20  # Clang Release C++20  
.\build_simple_clean.bat msvc release 23   # MSVC Release C++23
.\build_simple_clean.bat                   # GCC Release C++17 (por defecto)
```

### 2. `build_equivalent.sh` (Unix/Linux/macOS/WSL)
Script equivalente para sistemas Unix que replica exactamente la funcionalidad del .bat.

**Sintaxis:** `¿Usando CMake y ninja no se puede arreglar?`
```bash
./build_equivalent.sh [compilador] [tipo] [estandar]
```

**Ejemplos:**
```bash
./build_equivalent.sh gcc debug 17      # GCC Debug C++17
./build_equivalent.sh clang release 20  # Clang Release C++20
./build_equivalent.sh msvc release 23   # MSVC Release C++23 (solo WSL)
./build_equivalent.sh                   # GCC Release C++17 (por defecto)
```

## Parametros

- **compilador:** `gcc`, `clang`, `msvc`
- **tipo:** `debug`, `release` (case-insensitive) 
- **estandar:** `14`, `17`, `20`, `23` (C++ standard)

## Caracteristicas

### ✅ Funcionalidad Identica
- Ambos scripts implementan exactamente la misma logica
- Mismos parametros y comportamiento
- Mismos valores por defecto
- Misma deteccion de errores

### ✅ Compatibilidad Total
- **Windows:** `.bat` funciona en PowerShell, CMD, MSYS2
- **Unix/Linux:** `.sh` funciona en bash, zsh, WSL
- **macOS:** `.sh` soportado con Homebrew dependencies
- **MSYS2/MinGW:** Ambos scripts funcionan

### ✅ Automatizacion Completa
- Limpieza automatica del directorio build
- Configuracion automatica de CMake
- Compilacion paralela optimizada
- Deteccion automatica de errores
- Listado de ejecutables generados

### ✅ Soporte Multi-Compilador

#### GCC
```bash
# Windows (MSYS2)
.\build_simple_clean.bat gcc debug 17

# Unix/Linux/WSL
./build_equivalent.sh gcc debug 17
```

#### Clang  
```bash
# Windows (MSYS2)
.\build_simple_clean.bat clang release 20

# Unix/Linux/WSL
./build_equivalent.sh clang release 20
```

#### MSVC
```bash
# Windows (Script .bat con deteccion automatica de Visual Studio)
.\build_simple_clean.bat msvc release 23

# WSL (Script .sh con configuracion Visual Studio)
./build_equivalent.sh msvc release 23
```

## Requisitos del Sistema

### Windows
- **Visual Studio 2022** (para MSVC)
- **MSYS2/MinGW** (para GCC/Clang)
- **CMake 3.15+**
- **Ninja** (recomendado) o **MSBuild**
- **vcpkg** (automaticamente detectado en C:\vcpkg)

### Unix/Linux/macOS
- **GCC 9+** o **Clang 10+**
- **CMake 3.15+** 
- **Ninja** (recomendado) o **Make**
- **pkg-config** (para dependencias)

### WSL (Windows Subsystem for Linux)
- Cualquier distribucion Linux soportada
- Mismos requisitos que Linux nativo
- Opcional: Visual Studio para MSVC cross-compilation

## Instalacion Rapida

### Windows (MSYS2)
```bash
# Instalar dependencias completas
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-clang  
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja
pacman -S mingw-w64-ucrt-x86_64-boost
```

### Ubuntu/Debian
```bash
# Instalar dependencias completas
sudo apt update
sudo apt install build-essential cmake ninja-build
sudo apt install clang libboost-all-dev pkg-config
```

### macOS (Homebrew)
```bash
# Instalar dependencias completas
brew install cmake ninja boost
brew install gcc llvm
```

## Uso Tipico

### Desarrollo Diario (GCC Debug)
```bash
# Windows
.\build_simple_clean.bat gcc debug 17

# Unix/Linux 
./build_equivalent.sh gcc debug 17
```

### Release Build (Clang Optimizado)
```bash
# Windows  
.\build_simple_clean.bat clang release 20

# Unix/Linux
./build_equivalent.sh clang release 20
```

### Windows MSVC (Maximo Rendimiento)
```bash
# Solo Windows - Deteccion automatica de Visual Studio
.\build_simple_clean.bat msvc release 23
```

## Output Esperado

Ambos scripts generan el mismo output y los mismos ejecutables:

```
================================================
        AlgoritmiaCombinatoria Builder        
             Modo Simple v2.1                
================================================

Compilador: gcc
Build Type: Debug
Estandar C++: 17

[INFO] Limpiando build anterior...
[INFO] Configurando para GCC con C++17
[INFO] Configurando proyecto...
[INFO] Construyendo...
[SUCCESS] Construccion completada

Ejecutables generados:
AlgoritmiaCombinatoria.exe
factorial_demo.exe  
tests.exe

[INFO] Build completado en: build\
```

## Troubleshooting

### Error: CMake no encontrado
```bash
# Windows (MSYS2)
pacman -S mingw-w64-ucrt-x86_64-cmake

# Ubuntu/Debian
sudo apt install cmake

# macOS  
brew install cmake
```

### Error: Ninja no encontrado
```bash
# Windows (MSYS2)
pacman -S mingw-w64-ucrt-x86_64-ninja

# Ubuntu/Debian
sudo apt install ninja-build

# macOS
brew install ninja
```

### Error: Compilador no encontrado
```bash
# GCC en Windows (MSYS2)
pacman -S mingw-w64-ucrt-x86_64-gcc

# GCC en Ubuntu/Debian
sudo apt install build-essential

# Clang en Windows (MSYS2)  
pacman -S mingw-w64-ucrt-x86_64-clang

# Clang en Ubuntu/Debian
sudo apt install clang
```

### Error: Visual Studio no encontrado (MSVC)
- Instalar Visual Studio 2022 Community desde https://visualstudio.microsoft.com/
- Asegurarse de incluir "C++ CMake tools" y "MSVC compiler toolset"
- El script detecta automaticamente la instalacion en rutas estandar

## Integracion con IDEs

### Visual Studio Code
Ambos scripts se integran perfectamente con VS Code:

```json
// .vscode/tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build GCC Debug",
            "type": "shell",
            "command": "./build_equivalent.sh gcc debug 17",
            "group": "build",
            "problemMatcher": "$gcc"
        },
        {
            "label": "Build MSVC Release", 
            "type": "shell",
            "command": ".\\build_simple_clean.bat msvc release 23",
            "group": "build",
            "problemMatcher": "$msCompile"
        }
    ]
}
```

### CLion / Visual Studio
Los scripts generan archivos CMake compatibles que se pueden abrir directamente en cualquier IDE que soporte CMake.

## Arquitectura del Proyecto

```
AlgoritmiaCombinatoria/
├── build_simple_clean.bat     # Script Windows (funcional)
├── build_equivalent.sh        # Script Unix (equivalente) 
├── CMakeLists.txt            # Configuracion CMake multi-compiler
├── src/                      # Codigo fuente principal
├── include/                  # Headers con soporte __int128  
├── tests/                    # Tests con Catch2
└── build/                    # Directorio de compilacion (auto-generado)
    ├── AlgoritmiaCombinatoria.exe
    ├── factorial_demo.exe
    └── tests.exe
```

## Contribuciones

Este proyecto implementa scripts de construccion multiplataforma que demuestran:
- Equivalencia total entre Windows .bat y Unix .sh
- Automatizacion completa de CMake + Ninja
- Soporte nativo para GCC, Clang y MSVC
- Compatibilidad total entre diferentes sistemas operativos

Los scripts mantienen la misma sintaxis en ambas plataformas: `"¿Usando CMake y ninja no se puede arreglar?"`