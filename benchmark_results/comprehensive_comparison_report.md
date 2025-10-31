# Reporte de Comparación: CalculationResult vs std::optional

**Generado:** 20251031_161557

## Resumen de Comparaciones

- **Total de comparaciones:** 2
- **std::optional gana:** 1 tests
- **CalculationResult gana:** 1 tests
- **Speedup promedio:** 0.95x

## Comparaciones Detalladas

### Legacy CalculationResult Division vs Modern std::optional Division

| Métrica | CalculationResult | std::optional | Ratio |
|---------|-------------------|---------------|-------|
| Avg Time (ns) | 30.494 | 30.474 | 1.00x |
| Ops/Second | 32792906 | 32815073 | 1.00x |
| Success Rate | 99.5% | 99.5% | - |

**🤝 Empate técnico** - Diferencia < 5%

### Legacy CalculationResult Lookup vs Modern std::optional Division

| Métrica | CalculationResult | std::optional | Ratio |
|---------|-------------------|---------------|-------|
| Avg Time (ns) | 27.302 | 30.474 | 0.90x |
| Ops/Second | 36627621 | 32815073 | 1.12x |
| Success Rate | 63.8% | 99.5% | - |

**🏆 Winner: CalculationResult** - 11.6% más rápido

## Recomendaciones

🤝 **Recomendación: Migración basada en mantenibilidad**

Rendimiento similar en ambos sistemas. La decisión debe basarse en:

- Preferir std::optional por ser estándar
- Considerar el costo de migración
- Evaluar la experiencia del equipo

