#include <iostream>
#include <cmath>
#include <vector>
#include "matplotlibcpp.h"
#include <iomanip>
#include <fstream>

namespace plt = matplotlibcpp;

double f(double x, double t) { // Funcion f que cumple: f(x,t) = dx/dt
  return -x + sin(t);
}

double exacta(double t) { // Solucion exacta de x(t)
  return 0.5 * (sin(t) - cos(t)) + 1.5 * exp(-t);
}

void euler(std::vector<double>& t, std::vector<double>& x_num, double h, double x0) { // Metodo de solucion de la ec. diferencial de Euler
    int N = t.size();
    x_num[0] = x0;
    
    for (int i = 0; i < N - 1; i++) {
        x_num[i + 1] = x_num[i] + h * f(x_num[i], t[i]);
    }
}

void calcular_errores(const std::vector<double>& x_num, 
                     const std::vector<double>& t,
                     std::vector<double>& error,
                     double& error_max) {
    int N = t.size();
    error_max = 0.0;
    
    for (int i = 0; i < N; i++) {
        double x_exact = exacta(t[i]);
        error[i] = std::abs(x_num[i] - x_exact);
        if (error[i] > error_max) {
            error_max = error[i];
        }
    }
}


int main() {
  double h = 0.1;
  double t0 = 0.0;
  double tf = 10.0;
  double x0 = exacta(0.0);

  int N = static_cast<int>((tf - t0) / h + 1); // +1 para incluir tf
  std::vector<double> t(N);
  t.at(0) = t0;
  for (int i = 0; i < N; i++) {
    t[i] = t0 + i * h;
  }
  double x0 = exacta(0);
  std::cout<<euler(f, t, h, x0)<<std::endl
  return 0;
}
