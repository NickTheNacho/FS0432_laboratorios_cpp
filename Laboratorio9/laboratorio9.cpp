#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// Función f que cumple dx/dt = f(x,t)
double f(double x, double t) {
    return -x + sin(t);
}

// Solución analítica exacta
double exacta(double t) {
    return 0.5 * (sin(t) - cos(t)) + 1.5 * exp(-t);
}

// Método de Euler para solucion de la ED
void euler(const std::vector<double>& t, std::vector<double>& x_num, double h, double x0) {
    int N = t.size();
    x_num[0] = x0;
    
    for (int i = 0; i < N - 1; i++) {
        x_num[i + 1] = x_num[i] + h * f(x_num[i], t[i]);
    }
}

// Cálculo de errores
void calcular_errores(const std::vector<double>& x_num, 
                     const std::vector<double>& t,
                     std::vector<double>& error,
                     double& error_max) {
    int N = t.size();
    error_max = 0.0;
    
    for (int i = 0; i < N; i++) {
        double exact = exacta(t[i]);
        error[i] = std::abs(x_num[i] - exact);
        if (error[i] > error_max) {
            error_max = error[i];
        }
    }
}

// Generación de gráficas con matplotlib-cpp
void generar_graficas(const std::vector<double>& t,
                     const std::vector<double>& x_num,
                     const std::vector<double>& error) {
    
    // Cálculo de solución exacta para todos los puntos
    std::vector<double> x_exact(t.size());
    for (size_t i = 0; i < t.size(); i++) {
        x_exact[i] = exacta(t[i]);
    }
    
    // GRÁFICA 1: Comparación de soluciones
    plt::figure_size(1200, 500);  // Tamaño en píxeles
    
    // Subplot 1: Soluciones
    plt::subplot(1, 2, 1);
    plt::plot(t, x_num, "b-", {{"label", "Euler (h=0.1)"}, {"linewidth", "2"}});
    plt::plot(t, x_exact, "r--", {{"label", "Solución analítica"}, {"linewidth", "2"}});
    plt::xlabel("Tiempo t");
    plt::ylabel("x(t)");
    plt::title("Comparación: Método de Euler vs Solución Analítica");
    plt::legend();
    plt::grid(true);
    
    // Subplot 2: Error
    plt::subplot(1, 2, 2);
    plt::semilogy(t, error, "g-", {{"label", "Error absoluto"}, {"linewidth", "2"}});
    plt::xlabel("Tiempo t");
    plt::ylabel("Error absoluto E(t)");
    plt::title("Error absoluto local vs tiempo");
    plt::legend();
    plt::grid(true);
    
    plt::tight_layout();
    plt::save("solucion_euler.png");
    
    // GRÁFICA 2: Solo error
    plt::figure_size(800, 600);
    plt::semilogy(t, error, "g-", {{"linewidth", "2"}});
    plt::xlabel("Tiempo t");
    plt::ylabel("Error absoluto E(t)");
    plt::title("Error absoluto local en función del tiempo");
    plt::grid(true, {{"alpha", "0.3"}});
    plt::save("error_local.png");
    
    plt::show();
}

// Impresión de tabla
void imprimir_tabla(const std::vector<double>& t,
                   const std::vector<double>& x_num,
                   const std::vector<double>& error) {
    std::cout<<std::fixed<<std::setprecision(6);
    std::cout<<"\n";
    std::cout<<"t\t\tEuler\t\tExacta\t\tError"<<std::endl;
    std::cout<<"----------------------------------------------------------------"<<std::endl;
    
    for (size_t i = 0; i < t.size() && i < 20; i++) {  // Mostrar solo primeras 20 filas
        std::cout<<t[i]<<"\t" 
                 <<x_num[i]<<"\t"
                 <<exacta(t[i])<<"\t"
                 <<error[i]<<std::endl;
    }
    
    if (t.size() > 20) {
        std::cout<<"...\n(Se muestran solo las primeras 20 filas)"<<std::endl;
    }
}

int main() {
    std::cout<<"=== LABORATORIO 9: MÉTODO DE EULER ==="<<std::endl;
    std::cout<<"Ecuación: dx/dt = -x + sin(t)"<<std::endl;
    std::cout<<"Condición inicial: x(0) = 1"<<std::endl;
    std::cout<<"Intervalo: [0, 10]"<<std::endl;
    
    // Parámetros del problema
    double h = 0.1;
    double t0 = 0.0;
    double tf = 10.0;
    double x0 = exacta(0.0);  // x(0) = 1
    
    // Número de pasos (+1 para incluir t_f)
    int N = static_cast<int>((tf - t0) / h) + 1;
    
    std::cout<<"Tamaño de paso: h = "<<h<<std::endl;
    std::cout<<"Número de pasos: "<<N<<std::endl;
    
    // Crear vectores
    std::vector<double> t(N);
    std::vector<double> x_num(N);
    std::vector<double> error(N);
    
    // Llenar vector de tiempos
    for (int i = 0; i < N; i++) {
        t[i] = t0 + i * h;
    }
    
    // Aplicar método de Euler
    euler(t, x_num, h, x0);
    
    // Calcular errores
    double error_max;
    calcular_errores(x_num, t, error, error_max);
    
    std::cout<<"Error global máximo: "<<error_max<<std::endl;
    
    // Imprimir tabla de resultados
    imprimir_tabla(t, x_num, error);
    
    // Guardar resultados en archivo de texto
    std::ofstream output("output.txt");
    if (output.is_open()) {
        output<<"=== LABORATORIO 9: MÉTODO DE EULER ==="<<std::endl;
        output<<"Ecuación: dx/dt = -x + sin(t)"<<std::endl;
        output<<"Condición inicial: x(0) = 1"<<std::endl;
        output<<"Intervalo: [0, 10]"<<std::endl;
        output<<"Tamaño de paso: h = "<<h<<std::endl;
        output<<"Número de pasos: "<<N<<std::endl;
        output<<"Error global máximo: "<<error_max<<std::endl;
        output<<std::endl;
        output<<std::fixed<<std::setprecision(6);
        output<<"t\t\tEuler\t\tExacta\t\tError"<<std::endl;
        output<<"----------------------------------------------------------------"<<std::endl;
        
        for (int i = 0; i < N; i++) {
            output<<t[i]<<"\t" 
                   <<x_num[i]<<"\t"
                   <<exacta(t[i])<<"\t"
                   <<error[i]<<std::endl;
        }
        output.close();
        std::cout<<"\n✓ Resultados guardados en 'output.txt'"<<std::endl;
    } else {
        std::cerr<<"Error: No se pudo crear 'output.txt'"<<std::endl;
    }
    
    // Generar gráficas con matplotlib-cpp
    std::cout<<"\nGenerando gráficas con matplotlib-cpp..."<<std::endl;
    try {
        generar_graficas(t, x_num, error);
        std::cout<<"✓ Gráficas guardadas:"<<std::endl;
        std::cout<<"  - solucion_euler.png"<<std::endl;
        std::cout<<"  - error_local.png"<<std::endl;
    } catch (const std::exception& e) {
        std::cerr<<"Error al generar gráficas: "<<e.what()<<std::endl;
        std::cerr<<"Asegúrate de tener Python y matplotlib instalados"<<std::endl;
        return 1;
    }
    
    std::cout<<"\n=== PROGRAMA COMPLETADO EXITOSAMENTE ==="<<std::endl;
    
    return 0;
}
