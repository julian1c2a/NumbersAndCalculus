# Reporte de Comparación: CalculationResult vs std::optional

**Generado:** 20251031_225355

## Resumen de Comparaciones

- **Total de comparaciones:** 2
- **std::optional gana:** 1 tests
- **CalculationResult gana:** 1 tests
- **Speedup promedio:** 1.02x

## Comparaciones Detalladas

### Legacy CalculationResult Division vs Modern std::optional Division

| Métrica | CalculationResult | std::optional | Ratio |
|---------|-------------------|---------------|-------|
| Avg Time (ns) | 32.149 | 29.830 | 1.08x |
| Ops/Second | 31105166 | 33523748 | 0.93x |
| Success Rate | 99.5% | 99.5% | - |

**🏆 Winner: std::optional** - 7.8% más rápido

### Legacy CalculationResult Lookup vs Modern std::optional Division

| Métrica | CalculationResult | std::optional | Ratio |
|---------|-------------------|---------------|-------|
| Avg Time (ns) | 28.444 | 29.830 | 0.95x |
| Ops/Second | 35156304 | 33523748 | 1.05x |
| Success Rate | 63.6% | 99.5% | - |

**🤝 Empate técnico** - Diferencia < 5%

## Recomendaciones

🤝 **Recomendación: Migración basada en mantenibilidad**

Rendimiento similar en ambos sistemas. La decisión debe basarse en:

- Preferir std::optional por ser estándar
- Considerar el costo de migración
- Evaluar la experiencia del equipo

