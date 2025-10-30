# Configuración MSYS2 para AlgoritmiaCombinatoria

Este proyecto está optimizado para funcionar con MSYS2 UCRT64. Aquí tienes una guía completa para configurar tu entorno.

## Instalación de MSYS2

1. **Descargar MSYS2** desde [https://www.msys2.org/](https://www.msys2.org/)
2. **Instalar** en `C:\msys64` (ruta por defecto)
3. **Actualizar el sistema**:
   ```bash
   pacman -Syu
   ```

## Paquetes Necesarios

Ejecuta estos comandos en la terminal MSYS2 UCRT64:

```bash
# Herramientas de desarrollo básicas
pacman -S mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja
pacman -S mingw-w64-ucrt-x86_64-gdb

# Compiladores adicionales
pacman -S mingw-w64-ucrt-x86_64-clang
pacman -S mingw-w64-ucrt-x86_64-lldb

# Librerías del proyecto
pacman -S mingw-w64-ucrt-x86_64-boost
pacman -S mingw-w64-ucrt-x86_64-catch2

# Herramientas adicionales útiles
pacman -S mingw-w64-ucrt-x86_64-pkg-config
pacman -S mingw-w64-ucrt-x86_64-make
```

## Variables de Entorno

Agrega estas rutas a tu PATH del sistema:
- `C:\msys64\ucrt64\bin`
- `C:\msys64\usr\bin`

## Configuración de VS Code

El proyecto incluye configuración específica para MSYS2:

### IntelliSense
- Configuración `MSYS2-UCRT64` para GCC
- Configuración `MSYS2-Clang` para Clang
- Rutas de includes optimizadas para Boost

### Tareas de Construcción
- `Build with MSYS2 GCC`: Construcción con GCC
- `Build with MSYS2 Clang`: Construcción con Clang
- `Build with MSVC`: Construcción con Visual Studio (separada)

### Depuración
- `Debug with MSYS2 GDB`: Depuración con GDB
- `Debug with MSYS2 LLDB/Clang`: Depuración con LLDB
- `Debug Tests with MSYS2 GDB`: Depuración de pruebas

## Compilación Rápida

### Usando scripts automáticos:
```bash
# Con GCC (por defecto)
./build.sh --compiler gcc --type Release --test --run

# Con Clang
./build.sh --compiler clang --type Release --test --run

# En Windows PowerShell/CMD
build.bat --compiler gcc --type Release --test --run
```

### Usando CMake manualmente:
```bash
mkdir build && cd build

# Con GCC
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_BUILD_TYPE=Release

# Con Clang
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe -DCMAKE_BUILD_TYPE=Release

# Construir
cmake --build . --parallel 4

# Ejecutar pruebas
ctest --output-on-failure
```

## Verificación de la Instalación

Ejecuta estos comandos para verificar que todo está instalado correctamente:

```bash
# Verificar compiladores
g++ --version
clang++ --version

# Verificar herramientas
cmake --version
ninja --version

# Verificar debuggers
gdb --version
lldb --version

# Verificar Boost
pkg-config --modversion boost
```

## Solución de Problemas Comunes

### Error: "g++ no encontrado"
- Asegúrate de que `C:\msys64\ucrt64\bin` esté en tu PATH
- Reinicia VS Code tras cambiar el PATH

### Error: "boost/multiprecision no encontrado"
```bash
pacman -S mingw-w64-ucrt-x86_64-boost
```

### Error: "catch2 no encontrado"
```bash
pacman -S mingw-w64-ucrt-x86_64-catch2
```

### Error de linking con Boost
- Verifica que usas la terminal UCRT64, no MINGW64
- Asegúrate de que las librerías están en `C:\msys64\ucrt64\lib`

## Archivos de Cabecera (.hpp)

El proyecto usa archivos `.hpp` para las cabeceras C++:
- `include/combinatorics.hpp` - Interfaz principal
- `include/combinatorics_impl.hpp` - Implementaciones template

Esta es una convención común que diferencia claramente entre C (`.h`) y C++ (`.hpp`).

## Compatibilidad

- ✅ MSYS2 UCRT64 (Recomendado)
- ✅ MSYS2 MINGW64 (Funciona con ajustes menores)
- ✅ Visual Studio 2022 (MSVC)
- ⚠️ MSYS2 CLANG64 (Requiere ajustes de rutas)

## Notas Importantes

1. **Siempre usa la terminal UCRT64** para mejor compatibilidad con bibliotecas modernas
2. **Los archivos .exe se generan en build/** después de la compilación
3. **Las rutas están configuradas para la instalación estándar** en `C:\msys64`
4. **VS Code debe ejecutarse con acceso a las variables de entorno** actualizadas