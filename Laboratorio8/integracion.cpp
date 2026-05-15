#include <iostream>

using namespace std;

double integ_trapecios(double* array_f, int num_subint, double paso) {
  double aprox_integral = paso * (array_f[0] + array_f[num_subint]) / 2;
  for (int i = 1; i < (num_subint); i++) {
    aprox_integral += paso * array_f[i];
  }
  return aprox_integral;
}

double integ_simpson(double* array_f, int num_subint, double paso) {
    double suma = array_f[0] + array_f[num_subint];
  for (int i = 1; i < num_subint; i++) {
    if (i % 2 != 0) {
      suma += 4 * array_f[i]; // terminos de indice impar
    } else {
      suma += 2 * array_f[i]; // terminos de indice par
    }
  }
  return (paso / 3) * suma;
}
