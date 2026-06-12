# Laboratorio 12 — Ecuación de calor 2D

## Descripción

Simulación de la ecuación de calor en dos dimensiones mediante diferencias finitas explícitas (método de Euler hacia adelante):

```
∂u/∂t = κ (∂²u/∂x² + ∂²u/∂y²)
```

Dominio: [0,1]×[0,1]. Condiciones de frontera constantes:
- Bordes izquierdo y derecho: **T = 10**
- Bordes inferior y superior: **T = 5**

## Requisitos

- Python 3.x
- NumPy
- Matplotlib (con soporte Pillow para guardar GIF)

```bash
pip install numpy matplotlib pillow
```

## Cómo ejecutar

```bash
python heat_2d.py
```

El programa generará automáticamente la animación `calor_2d.gif` en el directorio de trabajo.

## Parámetros

| Parámetro | Valor |
|---|---|
| N (puntos de malla) | 50 × 50 |
| h (paso espacial) | 0.02 |
| κ (difusividad) | 1.0 |
| r (número de Courant) | 0.20 |
| dt | 8 × 10⁻⁵ |
| num_steps | 1250 |
| t_final | 0.1 |

El número de Courant r = κ·dt/h² = 0.20 < 0.25 garantiza la estabilidad del esquema explícito 2D.

## Archivos generados

| Archivo | Descripción |
|---|---|
| `calor_2d.gif` | Animación de la evolución temporal de la temperatura |
| `output.txt` | Ejemplo de salida de consola |
