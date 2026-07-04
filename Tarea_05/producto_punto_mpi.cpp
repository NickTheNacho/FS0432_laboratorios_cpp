#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <chrono>

// Función de inicialización provista en el enunciado
double f(double x) { 
    return std::sin(x); 
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 128000; // Tamaño total de los vectores
    
    // Validar que el tamaño sea divisible entre los procesos para simplificar
    if (N % size != 0) {
        if (rank == 0) {
            std::cerr << "Error: N (" << N << ") debe ser divisible por el número de procesos (" << size << ")." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int local_N = N / size; // Elementos por proceso
    std::vector<double> local_A(local_N);
    std::vector<double> local_B(local_N);

    // 1. Inicialización únicamente en el proceso con rank = 0
    if (rank == 0) {
        std::vector<double> A(N);
        std::vector<double> B(N);

        for (int i = 0; i < N; ++i) {
            A[i] = f((double)i);
            B[i] = 2.0 * f((double)i);
        }

        // Repartir los bloques al resto de los procesos
        for (int p = 0; p < size; ++p) {
            int start_idx = p * local_N;
            if (p == 0) {
                // El proceso 0 copia directamente a sus vectores locales
                std::copy(A.begin(), A.begin() + local_N, local_A.begin());
                std::copy(B.begin(), B.begin() + local_N, local_B.begin());
            } else {
                // Enviar usando etiquetas (tags) distintas para A y B
                MPI_Send(&A[start_idx], local_N, MPI_DOUBLE, p, 0, MPI_COMM_WORLD); // Tag 0 para A
                MPI_Send(&B[start_idx], local_N, MPI_DOUBLE, p, 1, MPI_COMM_WORLD); // Tag 1 para B
            }
        }
    } else {
        // Procesos esclavos reciben sus respectivos bloques usando los tags correspondientes
        MPI_Recv(local_A.data(), local_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(local_B.data(), local_N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Sincronizar antes de medir el tiempo
    MPI_Barrier(MPI_COMM_WORLD);
    auto start_time = std::chrono::high_resolution_clock::now();

    // 2. Calcular el producto punto local
    double local_dot_product = 0.0;
    for (int i = 0; i < local_N; ++i) {
        local_dot_product += local_A[i] * local_B[i];
    }

    // Sincronizar al terminar el cálculo local (para una medición de tiempo limpia)
    MPI_Barrier(MPI_COMM_WORLD);
    auto end_time = std::chrono::high_resolution_clock::now();
    double local_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

    // Obtener el tiempo máximo entre todos los procesos (el que dictamina la duración real del bucle)
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // 3. Reducción global usando MPI_Reduce con la operación MPI_SUM
    double global_dot_product = 0.0;
    MPI_Reduce(&local_dot_product, &global_dot_product, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // 4. Imprimir resultados únicamente desde el proceso con rank = 0
    if (rank == 0) {
        std::cout << "Resultado del producto punto: " << global_dot_product << std::endl;
        std::cout << "Tiempo de ejecucion del bucle (" << size << " procesos): " << max_time << " ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
