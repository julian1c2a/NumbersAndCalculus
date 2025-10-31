# Resultados de Benchmarking

**Timestamp:** 20251031_155954
**Total de tests:** 7

## Resumen Ejecutivo

- **Tiempo promedio general:** 17.359 ns por operación
- **Total de operaciones exitosas:** 60452010
- **Throughput promedio:** 57605772 ops/segundo

## Resultados Detallados

| Test Name | Iterations | Success | Failed | Avg Time (ns) | Min Time (ns) | Max Time (ns) | Ops/Second |
|-----------|------------|---------|--------|---------------|---------------|---------------|------------|
| Legacy CalculationResult Division | 500000 | 497420 | 2580 | 29.366 | 0.000 | 45200.000 | 34053218 |
| Modern std::optional Division | 500000 | 497420 | 2580 | 28.788 | 0.000 | 51200.000 | 34736454 |
| Legacy CalculationResult Lookup | 1000000 | 636060 | 363940 | 29.294 | 0.000 | 123100.000 | 34136916 |
| Modern std::optional Lookup | 1000000 | 636060 | 363940 | 27.453 | 0.000 | 37100.000 | 36426422 |
| Pure Functions (Square) | 50000000 | 50000000 | 0 | 1.001 | 1.001 | 1.001 | 999400359 |
| Lookup Tables Sequential Access | 5000000 | 5000000 | 0 | 0.000 | 0.000 | 0.000 | 0 |
| Lookup Tables Random Access | 5000000 | 3185050 | 1814950 | 5.615 | 5.615 | 5.615 | 178107077 |

## Análisis de Rendimiento

### Legacy CalculationResult Division vs Modern std::optional Division

- **Speedup:** 1.02x (Modern es más rápido)
- **Diferencia:** 0.578 ns

### Legacy CalculationResult Lookup vs Modern std::optional Division

- **Speedup:** 1.02x (Modern es más rápido)
- **Diferencia:** 0.506 ns

