# Reporte de Comparación: CalculationResult vs std::optional

**Generado:** 20251031_170159

## Resumen de Comparaciones

- **Total de comparaciones:** 2
- **std::optional gana:** 1 tests
- **CalculationResult gana:** 1 tests
- **Speedup promedio:** 1.00x

## Comparaciones Detalladas

### Legacy CalculationResult Division vs Modern std::optional Division

| Métrica | CalculationResult | std::optional | Ratio |
|---------|-------------------|---------------|-------|
| Avg Time (ns) | 30.208 | 28.702 | 1.05x |
| Ops/Second | 33103594 | 34841263 | 0.95x |
| Success Rate | 99.5% | 99.5% | - |

**🏆 Winner: std::optional** - 5.2% más rápido

### Legacy CalculationResult Lookup vs Modern std::optional Division

| Métrica | CalculationResult | std::optional | Ratio |
|---------|-------------------|---------------|-------|
| Avg Time (ns) | 27.397 | 28.702 | 0.95x |
| Ops/Second | 36499947 | 34841263 | 1.05x |
| Success Rate | 63.7% | 99.5% | - |

**🤝 Empate técnico** - Diferencia < 5%

## Recomendaciones

🤝 **Recomendación: Migración basada en mantenibilidad**

Rendimiento similar en ambos sistemas. La decisión debe basarse en:

- Preferir std::optional por ser estándar
- Considerar el costo de migración
- Evaluar la experiencia del equipo

