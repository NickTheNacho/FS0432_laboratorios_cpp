# Laboratorio 11 — Ecuación de Poisson 2D: Jacobi y Gauss-Seidel

## Descripción

Este laboratorio resuelve numéricamente la ecuación de Poisson 2D:

```
∂²u/∂x² + ∂²u/∂y² = f(x,y)
```

en el dominio [0,1]×[0,1] con condiciones de Dirichlet homogéneas, usando
dos métodos iterativos de diferencias finitas: **Jacobi** y **Gauss-Seidel**.

La función fuente utilizada es `f(x,y) = sin(πx)sin(πy)`, cuya solución
exacta es `u(x,y) = -sin(πx)sin(πy) / (2π²)`.

## Requisitos

- Python 3.x
- NumPy
- Matplotlib

Instalación de dependencias:
```bash
pip install numpy matplotlib
```

## Cómo ejecutar

### Método de Jacobi
```bash
python poisson_2d_jacobi.py
```

### Método de Gauss-Seidel
```bash
python poisson_2d_gauss_seidel.py
```

## Archivos generados

Cada script genera automáticamente las siguientes imágenes en el directorio
de trabajo:

| Archivo | Descripción |
|---|---|
| `solucion_jacobi_heatmap.png` | Mapa de calor de la solución numérica (Jacobi) |
| `solucion_exacta_heatmap.png` | Mapa de calor de la solución exacta |
| `error_absoluto_jacobi_heatmap.png` | Error absoluto punto a punto (Jacobi) |
| `error_convergencia_jacobi.png` | Error de convergencia vs iteración (Jacobi) |
| `solucion_gauss_seidel_heatmap.png` | Mapa de calor de la solución numérica (Gauss-Seidel) |
| `error_absoluto_gauss_seidel_heatmap.png` | Error absoluto punto a punto (Gauss-Seidel) |
| `error_convergencia_gauss_seidel.png` | Error de convergencia vs iteración (Gauss-Seidel) |

## Parámetros

| Parámetro | Valor |
|---|---|
| N (puntos de malla) | 50 |
| h (paso de malla) | 1/50 = 0.02 |
| Tolerancia | 1×10⁻⁸ |
| Máximo de iteraciones | 200 000 |

## Resultados

Ver `comparacion.txt` para una comparación detallada de ambos métodos.

- **Jacobi**: 4664 iteraciones, error máximo ≈ 1.16×10⁻⁵
- **Gauss-Seidel**: 2509 iteraciones, error máximo ≈ 1.42×10⁻⁵

Gauss-Seidel converge en aproximadamente la mitad de iteraciones que Jacobi.
