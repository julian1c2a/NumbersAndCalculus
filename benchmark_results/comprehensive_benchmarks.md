# Resultados de Benchmarking

**Timestamp:** 20251031_170159
**Total de tests:** 7

## Resumen Ejecutivo

- **Tiempo promedio general:** 17.333 ns por operación
- **Total de operaciones exitosas:** 60445170
- **Throughput promedio:** 57691905 ops/segundo

## Resultados Detallados

| Test Name | Iterations | Success | Failed | Avg Time (ns) | Min Time (ns) | Max Time (ns) | Ops/Second |
|-----------|------------|---------|--------|---------------|---------------|---------------|------------|
| Legacy CalculationResult Division | 500000 | 497525 | 2475 | 30.208 | 0.000 | 61600.000 | 33103594 |
| Modern std::optional Division | 500000 | 497525 | 2475 | 28.702 | 0.000 | 52800.000 | 34841263 |
| Legacy CalculationResult Lookup | 1000000 | 636940 | 363060 | 27.397 | 0.000 | 34700.000 | 36499947 |
| Modern std::optional Lookup | 1000000 | 636940 | 363060 | 27.744 | 0.000 | 37000.000 | 36043569 |
| Pure Functions (Square) | 50000000 | 50000000 | 0 | 1.101 | 1.101 | 1.101 | 908529272 |
| Lookup Tables Sequential Access | 5000000 | 5000000 | 0 | 0.000 | 0.000 | 0.000 | 0 |
| Lookup Tables Random Access | 5000000 | 3176240 | 1823760 | 6.182 | 6.182 | 6.182 | 161754715 |

## Análisis de Rendimiento

### Legacy CalculationResult Division vs Modern std::optional Division

- **Speedup:** 1.05x (Modern es más rápido)
- **Diferencia:** 1.507 ns

### Legacy CalculationResult Lookup vs Modern std::optional Division

- **Speedup:** 0.95x (Legacy es más rápido)
- **Diferencia:** 1.304 ns

