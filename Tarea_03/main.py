#!/usr/bin/env python3

import matplotlib
matplotlib.use('Agg')  # backend sin pantalla, evita segfault con Qt
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import lorenz_methods

def main():
    # Parámetros de integración
    h = 0.01
    t0 = 0.0
    tf = 40.0
    x0, y0, z0 = 1.0, 1.0, 1.0
    
    # Número de pasos
    N = int((tf - t0) / h) + 1
    t = np.linspace(t0, tf, N)
    
    print("=" * 60)
    print("TAREA 03: Sistema de Lorenz")
    print("=" * 60)
    print(f"Parámetros: σ = 10, ρ = 28, β = 8/3")
    print(f"Condiciones iniciales: x(0) = {x0}, y(0) = {y0}, z(0) = {z0}")
    print(f"Intervalo: [0, {tf}], h = {h}")
    print(f"Número de pasos: {N}")
    print("-" * 60)
    
    # Resolver con los tres métodos
    print("Resolviendo con método de Euler...")
    sol_euler = lorenz_methods.euler(h, t0, tf, x0, y0, z0)
    
    print("Resolviendo con método RK2...")
    sol_rk2 = lorenz_methods.rk2(h, t0, tf, x0, y0, z0)
    
    print("Resolviendo con método RK4...")
    sol_rk4 = lorenz_methods.rk4(h, t0, tf, x0, y0, z0)
    
    # Convertir a arrays de numpy para facilitar el manejo
    sol_euler = np.array(sol_euler)
    sol_rk2 = np.array(sol_rk2)
    sol_rk4 = np.array(sol_rk4)
    
    # =========================================================
    # Gráfica 3D: Método de Euler
    # =========================================================
    print("\nGenerando trayectoria_3d_euler.png...")
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(sol_euler[:, 0], sol_euler[:, 1], sol_euler[:, 2], 'b-', linewidth=0.8)
    ax.scatter(sol_euler[0, 0], sol_euler[0, 1], sol_euler[0, 2], 
               c='red', marker='o', s=50, label='Condición inicial')
    ax.set_xlabel('x(t)')
    ax.set_ylabel('y(t)')
    ax.set_zlabel('z(t)')
    ax.set_title('Sistema de Lorenz - Método de Euler')
    ax.legend()
    plt.savefig('trayectoria_3d_euler.png', dpi=150, bbox_inches='tight')
    plt.close()
    
    # =========================================================
    # Gráfica 3D: Método RK2
    # =========================================================
    print("Generando trayectoria_3d_rk2.png...")
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(sol_rk2[:, 0], sol_rk2[:, 1], sol_rk2[:, 2], 'g-', linewidth=0.8)
    ax.scatter(sol_rk2[0, 0], sol_rk2[0, 1], sol_rk2[0, 2], 
               c='red', marker='o', s=50, label='Condición inicial')
    ax.set_xlabel('x(t)')
    ax.set_ylabel('y(t)')
    ax.set_zlabel('z(t)')
    ax.set_title('Sistema de Lorenz - Método RK2')
    ax.legend()
    plt.savefig('trayectoria_3d_rk2.png', dpi=150, bbox_inches='tight')
    plt.close()
    
    # =========================================================
    # Gráfica 3D: Método RK4
    # =========================================================
    print("Generando trayectoria_3d_rk4.png...")
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(sol_rk4[:, 0], sol_rk4[:, 1], sol_rk4[:, 2], 'r-', linewidth=0.8)
    ax.scatter(sol_rk4[0, 0], sol_rk4[0, 1], sol_rk4[0, 2], 
               c='red', marker='o', s=50, label='Condición inicial')
    ax.set_xlabel('x(t)')
    ax.set_ylabel('y(t)')
    ax.set_zlabel('z(t)')
    ax.set_title('Sistema de Lorenz - Método RK4')
    ax.legend()
    plt.savefig('trayectoria_3d_rk4.png', dpi=150, bbox_inches='tight')
    plt.close()
    
    # =========================================================
    # Estudio de sensibilidad a condiciones iniciales
    # =========================================================
    print("\n" + "=" * 60)
    print("ESTUDIO DE SENSIBILIDAD A CONDICIONES INICIALES")
    print("=" * 60)
    
    epsilon = 1e-8
    x0_prime = x0 + epsilon
    y0_prime = y0
    z0_prime = z0
    
    print(f"Condición inicial original: r0 = ({x0}, {y0}, {z0})")
    print(f"Condición inicial perturbada: r0' = ({x0_prime}, {y0_prime}, {z0_prime})")
    print(f"Perturbación ε = {epsilon}")
    
    # Resolver con RK4 para ambas condiciones iniciales
    print("\nResolviendo con RK4 para condición original...")
    sol_rk4_original = np.array(lorenz_methods.rk4(h, t0, tf, x0, y0, z0))
    
    print("Resolviendo con RK4 para condición perturbada...")
    sol_rk4_perturbada = np.array(lorenz_methods.rk4(h, t0, tf, x0_prime, y0_prime, z0_prime))
    
    # Calcular la separación d(t) = ||r(t) - r'(t)||
    d = np.zeros(N)
    for i in range(N):
        diff = sol_rk4_original[i] - sol_rk4_perturbada[i]
        d[i] = np.linalg.norm(diff)
    
    # Imprimir separación final
    separacion_inicial = d[0]
    separacion_final = d[-1]
    
    print("\n" + "-" * 60)
    print(f"Separación inicial: {separacion_inicial:.6e}")
    print(f"Separación final:   {separacion_final:.6e}")
    print(f"Factor de amplificación: {separacion_final / separacion_inicial:.2e}")
    
    # Guardar separaciones en output.txt
    with open('output.txt', 'w') as f:
        f.write("=" * 80 + "\n")
        f.write("TAREA 03: Sistema de Lorenz\n")
        f.write("=" * 80 + "\n\n")
        
        f.write("PARÁMETROS:\n")
        f.write(f"  σ = 10\n")
        f.write(f"  ρ = 28\n")
        f.write(f"  β = 8/3 ≈ {8.0/3.0}\n")
        f.write(f"  h = {h}\n")
        f.write(f"  t ∈ [{t0}, {tf}]\n")
        f.write(f"  N = {N}\n\n")
        
        f.write("CONDICIONES INICIALES:\n")
        f.write(f"  r0 = ({x0}, {y0}, {z0})\n")
        f.write(f"  r0' = ({x0_prime}, {y0_prime}, {z0_prime})\n")
        f.write(f"  ε = {epsilon}\n\n")
        
        f.write("SEPARACIÓN d(t) = ||r(t) - r'(t)||:\n")
        f.write("-" * 80 + "\n")
        f.write(f"{'t':>12} {'d(t)':>20}\n")
        f.write("-" * 80 + "\n")
        
        # Mostrar algunos puntos intermedios (cada 200 pasos ~ cada 2 segundos)
        for i in range(0, N, 200):
            f.write(f"{t[i]:>12.6f} {d[i]:>20.6e}\n")
        
        # Último punto
        f.write(f"{t[-1]:>12.6f} {d[-1]:>20.6e}\n")
        f.write("-" * 80 + "\n")
        f.write(f"Separación inicial: {separacion_inicial:.6e}\n")
        f.write(f"Separación final:   {separacion_final:.6e}\n")
    
    print(f"\n✓ Resultados guardados en 'output.txt'")
    
    # =========================================================
    # Gráfica de sensibilidad
    # =========================================================
    print("Generando sensibilidad.png...")
    fig, ax = plt.subplots(figsize=(12, 6))
    
    # Usar escala semilogarítmica en y para mejor visualización
    # (excluyendo el valor cero inicial para evitar problemas con log)
    t_valid = t[1:]
    d_valid = d[1:]
    
    ax.semilogy(t_valid, d_valid, 'b-', linewidth=1.5)
    ax.set_xlabel('Tiempo t', fontsize=12)
    ax.set_ylabel('Separación d(t) = ||r(t) - r\'(t)||', fontsize=12)
    ax.set_title(f'Sensibilidad a condiciones iniciales (ε = {epsilon})', fontsize=14)
    ax.grid(True, alpha=0.3)
    
    # Marcar la separación final
    ax.axhline(y=separacion_final, color='r', linestyle='--', alpha=0.5, 
               label=f'Separación final: {separacion_final:.2e}')
    ax.legend()
    
    plt.tight_layout()
    plt.savefig('sensibilidad.png', dpi=150, bbox_inches='tight')
    plt.close()
    
    print("\n" + "=" * 60)
    print("ARCHIVOS GENERADOS:")
    print("=" * 60)
    print("  • trayectoria_3d_euler.png")
    print("  • trayectoria_3d_rk2.png")
    print("  • trayectoria_3d_rk4.png")
    print("  • sensibilidad.png")
    print("  • output.txt")
    print("\n¡Proceso completado exitosamente!")

if __name__ == "__main__":
    main()
