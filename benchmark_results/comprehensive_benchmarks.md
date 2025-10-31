# Resultados de Benchmarking

**Timestamp:** 20251031_161557
**Total de tests:** 7

## Resumen Ejecutivo

- **Tiempo promedio general:** 17.437 ns por operación
- **Total de operaciones exitosas:** 60454750
- **Throughput promedio:** 57349173 ops/segundo

## Resultados Detallados

| Test Name | Iterations | Success | Failed | Avg Time (ns) | Min Time (ns) | Max Time (ns) | Ops/Second |
|-----------|------------|---------|--------|---------------|---------------|---------------|------------|
| Legacy CalculationResult Division | 500000 | 497685 | 2315 | 30.494 | 0.000 | 102700.000 | 32792906 |
| Modern std::optional Division | 500000 | 497685 | 2315 | 30.474 | 0.000 | 33000.000 | 32815073 |
| Legacy CalculationResult Lookup | 1000000 | 637820 | 362180 | 27.302 | 0.000 | 36800.000 | 36627621 |
| Modern std::optional Lookup | 1000000 | 637820 | 362180 | 26.782 | 0.000 | 39100.000 | 37337813 |
| Pure Functions (Square) | 50000000 | 50000000 | 0 | 1.032 | 1.032 | 1.032 | 969367972 |
| Lookup Tables Sequential Access | 5000000 | 5000000 | 0 | 0.000 | 0.000 | 0.000 | 0 |
| Lookup Tables Random Access | 5000000 | 3183740 | 1816260 | 5.975 | 5.975 | 5.975 | 167358414 |

## Análisis de Rendimiento

### Legacy CalculationResult Division vs Modern std::optional Division

- **Speedup:** 1.00x (Modern es más rápido)
- **Diferencia:** 0.021 ns

### Legacy CalculationResult Lookup vs Modern std::optional Division

- **Speedup:** 0.90x (Legacy es más rápido)
- **Diferencia:** 3.172 ns

