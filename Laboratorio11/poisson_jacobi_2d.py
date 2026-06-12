# poisson_2d_jacobi.py
#
# Laboratorio 11
# Ecuacion de Poisson 2D usando el metodo de Jacobi
#
# Complete las secciones marcadas con TODO.

import numpy as np
import matplotlib.pyplot as plt


def fuente(x, y):
    return np.sin(np.pi * x) * np.sin(np.pi * y)


def solucion_exacta(x, y):
    return -np.sin(np.pi * x) * np.sin(np.pi * y) / (2.0 * np.pi**2)


def error_maximo(u, u_exacta):
    # TODO:
    # Calcule el error maximo entre la solucion numerica y la solucion exacta.
    #
    # Sugerencia:
    # return np.max(np.abs(...))

    return np.max(np.abs(u - u_exacta))


def graficar_mapa(u, titulo, nombre_archivo):
    plt.figure(figsize=(6, 5))

    # Se usa u.T porque u[i,j] representa u(x_i,y_j),
    # mientras que imshow interpreta el primer indice como eje vertical.
    plt.imshow(u.T, origin="lower", extent=[0, 1, 0, 1], cmap="viridis")

    plt.colorbar(label="u(x,y)")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(titulo)
    plt.tight_layout()
    plt.savefig(nombre_archivo, dpi=200)


def graficar_error_convergencia(errores, nombre_archivo):
    plt.figure(figsize=(6, 4))
    plt.plot(errores)
    plt.xlabel("Iteracion")
    plt.ylabel("Error de convergencia")
    plt.title("Error de convergencia")
    plt.tight_layout()
    plt.savefig(nombre_archivo, dpi=200)


def main():
    N = 50
    h = 1.0 / N
    tol = 1e-8
    max_iter = 200000

    x = np.linspace(0.0, 1.0, N + 1)
    y = np.linspace(0.0, 1.0, N + 1)

    X, Y = np.meshgrid(x, y, indexing="ij")

    f = fuente(X, Y)
    u_exacta = solucion_exacta(X, Y)

    u = np.zeros((N + 1, N + 1))
    u_new = np.zeros((N + 1, N + 1))

    errores_convergencia = []

    iteracion = 0
    error_conv = 1.0

    while error_conv > tol and iteracion < max_iter:

        # TODO:
        # Actualice los puntos interiores usando Jacobi.
        #
        # Para cada i,j interior:
        #
        # u_new[i,j] = 0.25 * (
        #     u[i+1,j] + u[i-1,j] + u[i,j+1] + u[i,j-1]
        #     - h**2 * f[i,j]
        # )
        #
        # Recuerde que los puntos interiores son:
        # i = 1, ..., N-1
        # j = 1, ..., N-1

        u_new[1:N,1:N] = 0.25 * (
            u[2:N+1, 1:N] + u[0:N-1, 1:N] +
            u[1:N, 2:N+1] + u[1:N, 0:N-1]
            - h**2 * f[1:N, 1:N]
        )

        # TODO:
        # Calcule el error de convergencia:
        #
        # error_conv = max |u_new - u|
        #
        # Sugerencia:
        # error_conv = np.max(np.abs(...))

        error_conv = np.max(np.abs(u_new[1:N,1:N] - u[1:N,1:N]))

        errores_convergencia.append(error_conv)

        # TODO:
        # Copie los valores nuevos hacia u.
        #
        # Importante:
        # Puede copiar solamente los puntos interiores,
        # para mantener fijas las condiciones de frontera.

        u[1:N, 1:N] = u_new[1:N, 1:N]

        # TODO:
        # Incremente el contador de iteraciones.

        iteracion += 1

    error_exacto = error_maximo(u, u_exacta)

    print("Metodo: Jacobi 2D")
    print("N =", N)
    print("h =", h)
    print("Tolerancia =", tol)
    print("Iteraciones =", iteracion)
    print("Error de convergencia final =", error_conv)
    print("Error maximo respecto a la solucion exacta =", error_exacto)

    error_absoluto = np.abs(u - u_exacta)

    graficar_mapa(
        u,
        "Jacobi 2D: solucion numerica",
        "solucion_jacobi_heatmap.png"
    )

    graficar_mapa(
        u_exacta,
        "Poisson 2D: solucion exacta",
        "solucion_exacta_heatmap.png"
    )

    graficar_error_convergencia(
        errores_convergencia,
        "error_convergencia_jacobi.png"
    )

    plt.show()

if __name__ == "__main__":
    main()
