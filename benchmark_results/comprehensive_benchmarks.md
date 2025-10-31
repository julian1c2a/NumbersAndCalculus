# Resultados de Benchmarking

**Timestamp:** 20251031_225355
**Total de tests:** 7

## Resumen Ejecutivo

- **Tiempo promedio general:** 18.041 ns por operación
- **Total de operaciones exitosas:** 60452170
- **Throughput promedio:** 55428597 ops/segundo

## Resultados Detallados

| Test Name | Iterations | Success | Failed | Avg Time (ns) | Min Time (ns) | Max Time (ns) | Ops/Second |
|-----------|------------|---------|--------|---------------|---------------|---------------|------------|
| Legacy CalculationResult Division | 500000 | 497570 | 2430 | 32.149 | 0.000 | 50400.000 | 31105166 |
| Modern std::optional Division | 500000 | 497570 | 2430 | 29.830 | 0.000 | 40000.000 | 33523748 |
| Legacy CalculationResult Lookup | 1000000 | 635650 | 364350 | 28.444 | 0.000 | 38400.000 | 35156304 |
| Modern std::optional Lookup | 1000000 | 635650 | 364350 | 28.639 | 0.000 | 47300.000 | 34917054 |
| Pure Functions (Square) | 50000000 | 50000000 | 0 | 1.138 | 1.138 | 1.138 | 878811846 |
| Lookup Tables Sequential Access | 5000000 | 5000000 | 0 | 0.000 | 0.000 | 0.000 | 0 |
| Lookup Tables Random Access | 5000000 | 3185730 | 1814270 | 6.088 | 6.088 | 6.088 | 164246764 |

## Análisis de Rendimiento

### Legacy CalculationResult Division vs Modern std::optional Division

- **Speedup:** 1.08x (Modern es más rápido)
- **Diferencia:** 2.319 ns

### Legacy CalculationResult Lookup vs Modern std::optional Division

- **Speedup:** 0.95x (Legacy es más rápido)
- **Diferencia:** 1.385 ns

