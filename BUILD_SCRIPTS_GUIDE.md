# 📋 GUÍA COMPLETA: Scripts de Build Disponibles

## ✅ **SCRIPTS DISPONIBLES EN TU PROYECTO**

### 1. **`build_simple.bat`** - ⭐ **VERSIÓN PRINCIPAL (MÁS ACTUALIZADA)**
```batch
# USO DESDE MSYS2 o PowerShell
.\build_simple.bat [compilador] [tipo] [cpp_version]

# EJEMPLOS:
.\build_simple.bat gcc debug 17        # ✅ GCC Debug C++17
.\build_simple.bat clang release 20    # ✅ Clang Release C++20  
.\build_simple.bat msvc release 23     # ✅ MSVC Release C++23 (mapea a latest)
.\build_simple.bat                     # ✅ Por defecto: GCC Release C++17
```

**✅ CARACTERÍSTICAS:**
- ✅ Soporte **COMPLETO** para compiladores: GCC, Clang, MSVC
- ✅ Soporte **COMPLETO** para estándares C++: 14, 17, 20, 23, latest
- ✅ Mapeo automático MSVC (C++23 → latest)
- ✅ Validación de argumentos y ayuda integrada
- ✅ **FUNCIONA PERFECTAMENTE** desde terminal MSYS2
- ✅ **RECOMENDADO** para uso diario

---

### 2. **`build_simple.sh`** - 📜 **VERSIÓN BASH LEGADO**
```bash
# USO DESDE MSYS2 bash
bash build_simple.sh [compilador] [tipo]

# EJEMPLOS:
bash build_simple.sh gcc debug         # ⚠️ Solo C++17 por defecto
bash build_simple.sh clang release     # ⚠️ Sin soporte versiones C++
```

**⚠️ LIMITACIONES:**
- ❌ **NO tiene soporte** para versiones C++ específicas
- ❌ **NO tiene mapeo MSVC**
- ❌ Solo compiladores básicos: GCC, Clang
- ❌ Funcionalidad limitada comparado con .bat
- 📜 **LEGADO** - mantener para compatibilidad

---

### 3. **`build_simple_msys2_wrapper.sh`** - 🔄 **WRAPPER EXPERIMENTAL**
```bash
# USO: Wrapper que llama al .bat desde bash
bash build_simple_msys2_wrapper.sh gcc debug 17
```

**🔄 CARACTERÍSTICAS:**
- 🔄 Llama internamente al `build_simple.bat`
- 🔄 Interfaz bash pero funcionalidad .bat
- 🔄 **EXPERIMENTAL** - en desarrollo

---

## 🎯 **RECOMENDACIONES DE USO**

### **DESDE TERMINAL MSYS2** (Tu caso):

#### **✅ OPCIÓN 1: Usar .bat directamente (RECOMENDADO)**
```bash
# Estos comandos funcionan perfectamente en MSYS2:
./build_simple.bat gcc debug 17
./build_simple.bat clang release 20
./build_simple.bat msvc release 23
```

**✅ VENTAJAS:**
- ✅ **Funcionalidad completa** (soporte C++ standards)
- ✅ **Probado y funcional**
- ✅ **Sin problemas de compatibilidad**
- ✅ **Misma sintaxis** que desde PowerShell

#### **⚠️ OPCIÓN 2: Usar .sh legado (LIMITADO)**
```bash  
# Funcionalidad básica solamente:
bash build_simple.sh gcc debug
bash build_simple.sh clang release
```

**⚠️ LIMITACIONES:**
- ❌ No especifica versión C++
- ❌ Solo funcionalidad básica

---

## 📊 **COMPARACIÓN RÁPIDA**

| Característica    | .bat                 | .sh        | wrapper              |
| ----------------- | -------------------- | ---------- | -------------------- |
| **Compiladores**  | GCC, Clang, MSVC     | GCC, Clang | GCC, Clang, MSVC     |
| **C++ Standards** | ✅ 14,17,20,23,latest | ❌ Solo 17  | ✅ 14,17,20,23,latest |
| **Mapeo MSVC**    | ✅ C++23→latest       | ❌ No       | ✅ C++23→latest       |
| **Desde MSYS2**   | ✅ Funciona           | ⚠️ Limitado | 🔄 Experimental       |
| **Estado**        | ✅ Principal          | 📜 Legado   | 🔄 Desarrollo         |
| **Recomendado**   | ⭐ **SÍ**             | ❌ No       | 🔄 Probar             |

---

## 🚀 **RESPUESTA A TU PREGUNTA**

**¿Los simple_build están tanto en .bat como en .sh?**

✅ **SÍ**, tienes ambas versiones:
- `build_simple.bat` - **VERSIÓN COMPLETA Y ACTUALIZADA**  
- `build_simple.sh` - **VERSIÓN BÁSICA LEGADO**

**¿Cuál usar desde MSYS2?**

⭐ **RECOMENDACIÓN:** Usa `build_simple.bat` directamente desde tu terminal MSYS2:

```bash
# ESTOS COMANDOS FUNCIONAN PERFECTAMENTE EN MSYS2:
./build_simple.bat gcc debug 17
./build_simple.bat clang release 20  
./build_simple.bat msvc release 23
```

**¿Por qué el .bat desde MSYS2?**
- ✅ **Funcionalidad completa** (soporte C++ standards)
- ✅ **Ya probado y funcional** 
- ✅ **Sin limitaciones** de la versión .sh
- ✅ **Mismo resultado** que desde PowerShell

---

## 💡 **RECOMENDACIÓN FINAL**

**Para tu uso diario desde MSYS2:**

```bash
# COMANDO RECOMENDADO (funciona perfecto):
./build_simple.bat gcc debug 17        # Tu favorito
./build_simple.bat clang release 20    # Alternativa
./build_simple.bat msvc release 23     # Para Windows específico
```

**¡El .bat funciona perfectamente desde tu terminal MSYS2 y tiene toda la funcionalidad que implementamos!** 🎉