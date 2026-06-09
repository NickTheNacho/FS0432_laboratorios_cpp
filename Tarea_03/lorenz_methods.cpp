#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <cmath>

namespace py = pybind11;

// Parámetros del sistema de Lorenz
const double SIGMA = 10.0;
const double RHO = 28.0;
const double BETA = 8.0 / 3.0;

// Función que calcula el lado derecho del sistema de Lorenz
// r = (x, y, z)
// El sistema es autónomo, por lo que no depende explícitamente de t
std::vector<double> lorenz(const std::vector<double>& r, double sigma, double rho, double beta) {
    double x = r[0];
    double y = r[1];
    double z = r[2];

    std::vector<double> drdt(3);
    drdt[0] = sigma * (y - x);
    drdt[1] = x * (rho - z) - y;
    drdt[2] = x * y - beta * z;

    return drdt;
}

// Método de Euler para sistemas de EDOs
std::vector<std::vector<double>> euler(double h, double t0, double tf,
                                        double x0, double y0, double z0) {
    int N = static_cast<int>((tf - t0) / h) + 1;
    std::vector<std::vector<double>> solucion(N, std::vector<double>(3));

    // Condición inicial
    solucion[0][0] = x0;
    solucion[0][1] = y0;
    solucion[0][2] = z0;

    std::vector<double> r(3);
    r[0] = x0;
    r[1] = y0;
    r[2] = z0;

    for (int i = 0; i < N - 1; i++) {
        // t no se usa porque el sistema es autónomo
        // [[maybe_unused]] double t = t0 + i * h;
        std::vector<double> drdt = lorenz(r, SIGMA, RHO, BETA);

        r[0] = r[0] + h * drdt[0];
        r[1] = r[1] + h * drdt[1];
        r[2] = r[2] + h * drdt[2];

        solucion[i + 1][0] = r[0];
        solucion[i + 1][1] = r[1];
        solucion[i + 1][2] = r[2];
    }

    return solucion;
}

// Método Runge-Kutta 2 (RK2)
std::vector<std::vector<double>> rk2(double h, double t0, double tf,
                                      double x0, double y0, double z0) {
    int N = static_cast<int>((tf - t0) / h) + 1;
    std::vector<std::vector<double>> solucion(N, std::vector<double>(3));

    // Condición inicial
    solucion[0][0] = x0;
    solucion[0][1] = y0;
    solucion[0][2] = z0;

    std::vector<double> r(3);
    r[0] = x0;
    r[1] = y0;
    r[2] = z0;

    for (int i = 0; i < N - 1; i++) {
        // k1 = F(r_n) - no depende de t
        std::vector<double> k1 = lorenz(r, SIGMA, RHO, BETA);

        // Calcular r_n + (h/2)*k1
        std::vector<double> r_temp(3);
        r_temp[0] = r[0] + (h / 2.0) * k1[0];
        r_temp[1] = r[1] + (h / 2.0) * k1[1];
        r_temp[2] = r[2] + (h / 2.0) * k1[2];

        // k2 = F(r_n + (h/2)*k1) - no depende de t
        std::vector<double> k2 = lorenz(r_temp, SIGMA, RHO, BETA);

        // Actualización: r_{n+1} = r_n + h * k2
        r[0] = r[0] + h * k2[0];
        r[1] = r[1] + h * k2[1];
        r[2] = r[2] + h * k2[2];

        solucion[i + 1][0] = r[0];
        solucion[i + 1][1] = r[1];
        solucion[i + 1][2] = r[2];
    }

    return solucion;
}

// Método Runge-Kutta 4 (RK4)
std::vector<std::vector<double>> rk4(double h, double t0, double tf,
                                      double x0, double y0, double z0) {
    int N = static_cast<int>((tf - t0) / h) + 1;
    std::vector<std::vector<double>> solucion(N, std::vector<double>(3));

    // Condición inicial
    solucion[0][0] = x0;
    solucion[0][1] = y0;
    solucion[0][2] = z0;

    std::vector<double> r(3);
    r[0] = x0;
    r[1] = y0;
    r[2] = z0;

    for (int i = 0; i < N - 1; i++) {
        // k1 = F(r_n) - no depende de t
        std::vector<double> k1 = lorenz(r, SIGMA, RHO, BETA);

        // r_n + (h/2)*k1
        std::vector<double> r_temp1(3);
        r_temp1[0] = r[0] + (h / 2.0) * k1[0];
        r_temp1[1] = r[1] + (h / 2.0) * k1[1];
        r_temp1[2] = r[2] + (h / 2.0) * k1[2];

        // k2 = F(r_n + (h/2)*k1) - no depende de t
        std::vector<double> k2 = lorenz(r_temp1, SIGMA, RHO, BETA);

        // r_n + (h/2)*k2
        std::vector<double> r_temp2(3);
        r_temp2[0] = r[0] + (h / 2.0) * k2[0];
        r_temp2[1] = r[1] + (h / 2.0) * k2[1];
        r_temp2[2] = r[2] + (h / 2.0) * k2[2];

        // k3 = F(r_n + (h/2)*k2) - no depende de t
        std::vector<double> k3 = lorenz(r_temp2, SIGMA, RHO, BETA);

        // r_n + h*k3
        std::vector<double> r_temp3(3);
        r_temp3[0] = r[0] + h * k3[0];
        r_temp3[1] = r[1] + h * k3[1];
        r_temp3[2] = r[2] + h * k3[2];

        // k4 = F(r_n + h*k3) - no depende de t
        std::vector<double> k4 = lorenz(r_temp3, SIGMA, RHO, BETA);

        // Actualización: r_{n+1} = r_n + (h/6)*(k1 + 2*k2 + 2*k3 + k4)
        r[0] = r[0] + (h / 6.0) * (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]);
        r[1] = r[1] + (h / 6.0) * (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]);
        r[2] = r[2] + (h / 6.0) * (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]);

        solucion[i + 1][0] = r[0];
        solucion[i + 1][1] = r[1];
        solucion[i + 1][2] = r[2];
    }

    return solucion;
}

// Módulo pybind11
PYBIND11_MODULE(lorenz_methods, m) {
    m.doc() = "Módulo con métodos numéricos para el sistema de Lorenz";

    m.def("lorenz", &lorenz, "Función del lado derecho del sistema de Lorenz",
          py::arg("r"), py::arg("sigma"), py::arg("rho"), py::arg("beta"));

    m.def("euler", &euler, "Método de Euler para el sistema de Lorenz",
          py::arg("h"), py::arg("t0"), py::arg("tf"),
          py::arg("x0"), py::arg("y0"), py::arg("z0"));

    m.def("rk2", &rk2, "Método Runge-Kutta 2 para el sistema de Lorenz",
          py::arg("h"), py::arg("t0"), py::arg("tf"),
          py::arg("x0"), py::arg("y0"), py::arg("z0"));

    m.def("rk4", &rk4, "Método Runge-Kutta 4 para el sistema de Lorenz",
          py::arg("h"), py::arg("t0"), py::arg("tf"),
          py::arg("x0"), py::arg("y0"), py::arg("z0"));
}
