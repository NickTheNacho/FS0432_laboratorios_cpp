#include <iostream>
#include <cmath>
#include "funciones.h"
#include "integracion.h"

using namespace std;

int main() {
  
  int tamano_array;

  // Solicitud al usuario de introducir el numero de subintervalos deseados
  cout<<"Introduzca numero de subintervalos"<<endl;
  cin>>tamano_array;
  while (tamano_array % 2 != 0) {
    // Asegurar que el numero de subintervalos sea par para que la Regla de Simpson funcione
    cout<<"Por favor utilice un numero par"<<endl;
    cin>>tamano_array;
  }
    // Creacion de los arrays dinamicos utilizando el input del usuario
    double* puntero_array_x = new double[tamano_array + 1];
    double* puntero_array_f = new double[tamano_array + 1];
    // Asignacion de valores a cada uno de los elementos de ambos arrays
    puntero_array_x[0] = 0.0;
    puntero_array_x[tamano_array] = 1.0;
    double paso = (puntero_array_x[tamano_array] - puntero_array_x[0]) / tamano_array;
    for (int i = 1; i <= (tamano_array - 1); i++) {
      puntero_array_x[i] = paso * i;
    }
    for (int i = 0; i <= tamano_array; i++) {
      puntero_array_f[i] = funcion_pi(puntero_array_x[i]);
    }
    delete[] puntero_array_x;

    // Llamar funciones de integracion para calcular resultados y errores
    const double pi = M_PI;
    double trapecios_comp = integ_trapecios(puntero_array_f, tamano_array, paso);
    double simpson_comp = integ_simpson(puntero_array_f, tamano_array, paso);
    double err_trap = abs(pi - trapecios_comp);
    double err_simp = abs(pi - simpson_comp);
    
    delete[] puntero_array_f;

    // Salida del programa que imprime resultados y errores de cada metodo
    cout<<"Integral de 4/(1+x^2) en [0, 1]"<<endl;
    cout<<" "<<endl;
    cout<<"Trapecio compuesto: "<<trapecios_comp<<endl;
    cout<<"Simpson compuesto: "<<simpson_comp<<endl;
    cout<<"Valor exacto: "<<pi<<endl;
    cout<<" "<<endl;
    cout<<"Error trapecio: "<<err_trap<<endl;
    cout<<"Error Simpson: "<<err_simp<<endl;

  return 0;
}
