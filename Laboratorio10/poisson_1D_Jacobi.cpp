/* poisson_1d_Jacobi.cpp

Repositorio oficial Matplotlib-cpp:
https://github.com/lava/matplotlib-cpp

Recuerde descargar archivo \texttt{matplotlibcpp.h} y 
colocarlo en el mismo directorio que este código.

Compilar como:

g++ poisson_1d_Jacobi.cpp -o poisson_1d_Jacobi.x \
    -I$CONDA_PREFIX/include/python3.11 \
    -I$(python -c "import numpy; print(numpy.get_include())") \
    -L$CONDA_PREFIX/lib \
    -lpython3.11 \
    -Wl,-rpath,$CONDA_PREFIX/lib
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

double fuente(double x) 
{
    return std::sin(M_PI * x);
}

double solucion_exacta(double x) 
{
    return -std::sin(M_PI * x) / (M_PI * M_PI);
}

int main() 
{
    const int N = 10; // numero de subintervalos
    const double a = 0.0;
    const double b = 1.0;
    const double h = (b - a) / N;

    const double alpha = 0.0;       // u(a)
    const double beta = 0.0;        // u(b)

    const int max_iter = 100000;
    const double tol = 1e-10;

    std::vector<double> x(N + 1);
    std::vector<double> u(N + 1, 0.0);
    std::vector<double> u_new(N + 1, 0.0); // Vector auxiliar necesario para Jacobi
    std::vector<double> u_exacta(N + 1);

    for (int i = 0; i <= N; i++) 
    {
        x[i] = a + i * h;
        u_exacta[i] = solucion_exacta(x[i]);
    }

    // Condiciones de frontera en ambos vectores
    u[0] = alpha;
    u[N] = beta;
    u_new[0] = alpha;
    u_new[N] = beta;

    int iter = 0;
    double error = 1.0;

    while (iter < max_iter && error > tol) 
    {

    // Actualizar los puntos interiores usando el método de Jacobi
    for (int i = 1; i < N; i++) 
    {
        u_new[i] = (u[i-1] + u[i+1] - h * h * fuente(x[i])) / 2.0;
    }
    
    // Calcular el error máximo entre iteraciones
    error = 0.0;
    for (int i = 1; i < N; i++) 
    {
        error = std::max(error, std::abs(u_new[i] - u[i]));
    }
    
    // Copiar u_new a u para la siguiente iteración
    for (int i = 1; i < N; i++) 
    {
        u[i] = u_new[i];
    }
    
    // Las condiciones de frontera ya están fijas en u[0] y u[N]
    
    iter++;

    }

    std::cout << "Iteraciones: " << iter << std::endl;
    std::cout << "Error final: " << error << std::endl;

    double error_max = 0.0;
    for (int i = 0; i <= N; i++) 
    {
        error_max = std::max(error_max, std::abs(u[i] - u_exacta[i]));
    }

    std::cout << "Error maximo contra solucion exacta: "
              << error_max << std::endl;


    /*
    
    Grafique la solución numérica y 
    la solución  exacta usando matplotlib-cpp.

    */

    // Crear figura 1: Solución numérica vs solución exacta
    plt::figure_size(800, 600);
    plt::plot(x, u, "b-", {{"label", "Solución Jacobi"}, {"linewidth", "2"}});
    plt::plot(x, u_exacta, "r--", {{"label", "Solución exacta"}, {"linewidth", "2"}});
    plt::title("Ecuación de Poisson 1D - Método de Jacobi");
    plt::xlabel("x");
    plt::ylabel("u(x)");
    plt::legend();
    plt::grid(true);
    plt::save("solucion_jacobi.png");

    // Crear figura 2: Error de convergencia
    // Nota: Necesitas almacenar los errores durante las iteraciones

    // Para almacenar los errores, añade esto ANTES del bucle while:
    std::vector<double> errores_historial;

    // Y DENTRO del bucle while, después de calcular 'error', añade:
    // errores_historial.push_back(error);

    // Luego de que termine el bucle, creas el vector de iteraciones:
    std::vector<int> iteraciones(errores_historial.size());
    for (size_t k = 0; k < errores_historial.size(); k++)
    {
        iteraciones[k] = k + 1; // iteración 1, 2, 3, ...
    }

    // Graficar el error de convergencia
    plt::figure_size(800, 600);
    plt::semilogy(iteraciones, errores_historial, "g-", {{"label", "Error de convergencia"}, {"linewidth", "2"}});
    plt::title("Convergencia del Método de Jacobi");
    plt::xlabel("Número de iteración (k)");
    plt::ylabel("Error máximo ||u^{(k+1)} - u^{(k)}||_\\infty");
    plt::legend();
    plt::grid(true);
    plt::save("error_jacobi.png");

    // Opcional: Mostrar las figuras en pantalla
    plt::show();

    return 0;
}
