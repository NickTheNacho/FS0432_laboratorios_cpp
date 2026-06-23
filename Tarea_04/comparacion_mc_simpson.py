"""
Compara integración por Simpson vs Monte Carlo.

I = int_0^1 ... int_0^1 prod_i sin(pi x_i) dx_1 ... dx_d
"""

import numpy as np
from scipy.integrate import simpson
import time
import matplotlib.pyplot as plt

d = 3  # dimensión del problema
valor_analitico = (2.0 / np.pi)**d


print(f"--- Integración en d={d} ---")
print(f"Analítico:   {valor_analitico:.8f}")
# ---------------------------------------------------------
# MÉTODO DE MONTE CARLO
# ---------------------------------------------------------

N_total_mc = 10**6


t0_mc = time.time()

'''
TODO
Implemente acá el método de Monte Carlo para la integral I.
integral_mc =
'''

# Se generan N_total_mc puntos aleatorios uniformes en [0,1]^d.
# Cada punto es un vector (x_1,...,x_d); la función evaluada en ese
# punto vale prod_{i=1}^d sin(pi * x_i).
# El estimador de Monte Carlo es el promedio de esos N valores.

muestras    = np.random.uniform(0.0, 1.0, size=(N_total_mc, d))  # shape (N, d)
valores     = np.prod(np.sin(np.pi * muestras), axis=1)           # shape (N,)
integral_mc = np.mean(valores)                                     # escalar

t1_mc = time.time()
error_mc = abs(integral_mc - valor_analitico)   

print(
    f"Monte Carlo: {integral_mc:.8f} "
    f"(Error: {error_mc:.8f}, Tiempo: {t1_mc - t0_mc:.4f}s)"
)

# ---------------------------------------------------------
# MÉTODO DE SIMPSON
# ---------------------------------------------------------

N_simpson = 10
N_total_simpson = N_simpson**d

t0_simpson = time.time()

x_1d = np.linspace(0, 1, N_simpson)
malla = np.meshgrid(*[x_1d] * d, indexing="ij")

Z = np.prod([np.sin(np.pi * m) for m in malla], axis=0)

integral_simpson = Z
for _ in range(d):
    integral_simpson = simpson(integral_simpson, x=x_1d, axis=0)

t1_simpson = time.time()
error_simpson = abs(integral_simpson - valor_analitico)

print(
    f"Simpson:     {integral_simpson:.8f} "
    f"(Error: {error_simpson:.8f}, Tiempo: {t1_simpson - t0_simpson:.4f}s)"
)

# =========================================================
# EXPLORACIÓN NUMÉRICA
# =========================================================

print("\n--- Exploración: error y tiempo vs dimensionalidad ---")

# Para Simpson se usa la malla completa (n^d puntos).
# Se limita a d<=7 porque d=8 con n=10 requiere 10^8 puntos (~800 MB).
DIMS_SIMP = [1, 2, 3, 4, 5, 6, 7]
DIMS_MC   = [1, 2, 3, 4, 5, 6, 7, 8, 16, 32]
N_MC      = 10**6
N_SIMP    = 10
N_REP     = 5

mc_errores   = []
mc_tiempos   = []
simp_errores = []
simp_tiempos = []

# --- Simpson vs dimensionalidad ---
for dim in DIMS_SIMP:
    exacto = (2.0 / np.pi)**dim
    t0 = time.time()
    x = np.linspace(0, 1, N_SIMP)
    malla = np.meshgrid(*[x] * dim, indexing="ij")
    Z = np.prod([np.sin(np.pi * m) for m in malla], axis=0)
    est = Z
    for _ in range(dim):
        est = simpson(est, x=x, axis=0)
    elapsed = time.time() - t0
    simp_errores.append(abs(est - exacto))
    simp_tiempos.append(elapsed)
    print(f"  Simpson d={dim}: est={est:.6f}  err={abs(est-exacto):.2e}  t={elapsed:.4f}s")

# --- Monte Carlo vs dimensionalidad ---
for dim in DIMS_MC:
    exacto = (2.0 / np.pi)**dim
    errs, ts = [], []
    for _ in range(N_REP):
        t0 = time.time()
        X   = np.random.uniform(0.0, 1.0, size=(N_MC, dim))
        est = np.mean(np.prod(np.sin(np.pi * X), axis=1))
        ts.append(time.time() - t0)
        errs.append(abs(est - exacto))
    mc_errores.append(np.mean(errs))
    mc_tiempos.append(np.mean(ts))
    print(f"  MC      d={dim:>2}: err={np.mean(errs):.2e}  t={np.mean(ts):.4f}s")

# --- Convergencia MC: error vs N para d=3 ---
print("\n--- Convergencia Monte Carlo (d=3) ---")
N_vals = [100, 500, 1_000, 5_000, 10_000, 50_000, 100_000, 500_000, 1_000_000]
conv_errores = []
exacto_3 = (2.0 / np.pi)**3
for N in N_vals:
    errs = [abs(np.mean(np.prod(np.sin(np.pi * np.random.uniform(0,1,size=(N,3))),axis=1)) - exacto_3)
            for _ in range(20)]
    conv_errores.append(np.mean(errs))
    print(f"  N={N:>9d}  error medio={conv_errores[-1]:.4e}")

# =========================================================
# FIGURAS
# =========================================================

plt.style.use("seaborn-v0_8-whitegrid")

# Figura 1: Error vs dimensionalidad
fig, ax = plt.subplots(figsize=(7, 4.5))
ax.semilogy(DIMS_SIMP, simp_errores, "s--", color="#E91E63", linewidth=2,
            markersize=7, label=f"Simpson (n={N_SIMP} por dim, malla completa)")
ax.semilogy(DIMS_MC,   mc_errores,   "o-",  color="#2196F3", linewidth=2,
            markersize=7, label=f"Monte Carlo (N={N_MC:,})")
ax.set_xlabel("Dimensión d", fontsize=12)
ax.set_ylabel("Error absoluto", fontsize=12)
ax.set_title("Error numérico vs. dimensionalidad", fontsize=13)
ax.legend()
fig.tight_layout()
fig.savefig(fname='error_vs_dim')
plt.close(fig)

# Figura 2: Tiempo vs dimensionalidad
fig, ax = plt.subplots(figsize=(7, 4.5))
ax.semilogy(DIMS_SIMP, [t*1e3 for t in simp_tiempos], "s--", color="#E91E63",
            linewidth=2, markersize=7, label=f"Simpson (n={N_SIMP} por dim)")
ax.semilogy(DIMS_MC,   [t*1e3 for t in mc_tiempos],   "o-",  color="#2196F3",
            linewidth=2, markersize=7, label=f"Monte Carlo (N={N_MC:,})")
ax.set_xlabel("Dimensión d", fontsize=12)
ax.set_ylabel("Tiempo (ms)", fontsize=12)
ax.set_title("Tiempo de ejecución vs. dimensionalidad", fontsize=13)
ax.legend()
fig.tight_layout()
fig.savefig(fname='t_vs_dim')
plt.close(fig)

# Figura 3: Convergencia MC
fig, ax = plt.subplots(figsize=(7, 4.5))
N_arr = np.array(N_vals, dtype=float)
ax.loglog(N_arr, conv_errores, "o-", color="#4CAF50", linewidth=2,
          markersize=7, label="Error MC (d=3)")
ref = conv_errores[0] * np.sqrt(N_vals[0]) / np.sqrt(N_arr)
ax.loglog(N_arr, ref, "k--", linewidth=1.5, label=r"$\propto 1/\sqrt{N}$")
ax.set_xlabel("N (número de muestras)", fontsize=12)
ax.set_ylabel("Error absoluto medio", fontsize=12)
ax.set_title("Convergencia de Monte Carlo (d=3)", fontsize=13)
ax.legend()
fig.tight_layout()
fig.savefig(fname='conver_MC')
plt.close(fig)

