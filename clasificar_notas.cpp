// clasificar_notas.cpp
// FS0432 - Física Computacional
// Laboratorio 7
// Nicholas Snodgrass

#include <iostream>

using namespace std;

void mostrarNotas(double* notas, int cant_notas)
{
    cout << "\nNotas validas ingresadas:\n";

    for (int i = 0; i < cant_notas; i++)
    {
        cout << "Nota " << i + 1 << ": " << notas[i] << endl;
    }
}

// Función para clasificar el rendimiento de los estudiantes
// Reprobado: menor a 67.5
// Aprobado: entre 67.5 y 89.9
// Sobresaliente: 90 o más

void clasificarRendimiento(double* notas, int cant_notas, int& reprobados, int& aprobados, int& sobresalientes)
{
    // Inicializar contadores
    reprobados = 0;
    aprobados = 0;
    sobresalientes = 0;

    for (int i = 0; i < cant_notas; i++)
    {
        if (notas[i] < 67.5)
        {
            reprobados++;
        }
        else if (notas[i] < 90.0)
        {
            aprobados++;
        }
        else
        {
            sobresalientes++;
        }
    }
}

// Función para calcular el promedio del grupo

double calcularPromedio(double* notas, int cant_notas)
{
    double suma = 0.0;

    for (int i = 0; i < cant_notas; i++)
    {
        suma += notas[i];
    }

    if (cant_notas > 0)
    {
        return suma / cant_notas;
    }
    else
    {
        return 0.0;
    }
}

// Función para encontrar nota máxima y mínima


void encontrarMaxMin(double* notas, int cant_notas, double& maxima, double& minima)
{
    maxima = notas[0];
    minima = notas[0];

    for (int i = 1; i < cant_notas; i++)
    {
        if (notas[i] > maxima)
        {
            maxima = notas[i];
        }

        if (notas[i] < minima)
        {
            minima = notas[i];
        }
    }
}

// Función principal

int main()
{
    // Arreglo estático de 10 elementos
    double notas[10];

    int cant_notas = 0;
    int intentos = 0;

    double entrada;

    cout << "===== SISTEMA DE NOTAS =====\n";

    // Ingreso de notas usando do-while
    
    do
    {
        cout << "\nIngrese una nota entre 0 y 100 ";
        cout << "(o -1 para terminar): ";

        cin >> entrada;

        intentos++;

        if (entrada == -1)
        {
            break;
        }

        // Validacion de rango
        if (entrada < 0.0 || entrada > 100.0)
        {
            cout << "Nota inválida. Intente nuevamente.\n";
            continue;
        }

        // Guardar nota
        if (cant_notas < 10)
        {
            notas[cant_notas] = entrada;
            cant_notas++;
        }
        else
        {
            cout << "Se alcanzó el límite de 10 notas.\n";
            break;
        }

    } while (cant_notas < 10 && intentos < 15);

    // Mostrar notas ingresadas
    mostrarNotas(notas, cant_notas);

    // Clasificación de rendimiento
    int reprobados;
    int aprobados;
    int sobresalientes;

    clasificarRendimiento(notas, cant_notas, reprobados, aprobados, sobresalientes);

    cout << "\nClasificación del grupo:\n";
    cout << "Reprobados: " << reprobados << endl;
    cout << "Aprobados: " << aprobados << endl;
    cout << "Sobresalientes: " << sobresalientes << endl;

    // Promedio del grupo
    double promedio = calcularPromedio(notas, cant_notas);

    cout << "\nPromedio del grupo: " << promedio << endl;

    // Nota máxima y mínima
    if (cant_notas > 0)
    {
        double maxima;
        double minima;

        encontrarMaxMin(notas, cant_notas, maxima, minima);

        cout << "\nNota máxima: " << maxima << endl;
        cout << "Nota mínima: " << minima << endl;
    }
    else
    {
        cout << "\nNo se ingresaron notas válidas.\n";
    }

    return 0;
}
