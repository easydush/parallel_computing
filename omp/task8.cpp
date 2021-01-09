/* Используя возможности OpenMP, написать программу умножения матрицы на вектор.
Сравнить время выполнения последовательной и параллельных программ. */

#include <cstdio>
#include <omp.h>

#define N 116
#define M 169

int main() {
    static long matrix[N][M];
    static long vector[M];

    // vector[i] = i*2, i=(1,M)
    for (int i = 0; i < M; i++) {
        vector[i] = i;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i][j] = i + 1;
        }
    }
    long result_vector[N];

    double start_time = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        result_vector[i] = 0;
        for (int j = 0; j < M; j++) {
            result_vector[i] += matrix[i][j] * vector[j];
        }
    }
    double end_time = omp_get_wtime();
    double result_time = (end_time - start_time);

    printf("subseq total time: %f\n", result_time);

    double result_time_parallel;
    double start_time_parallel;
    double end_time_parallel;

#pragma omp parallel num_threads(4)
    {
        start_time_parallel = omp_get_wtime();

#pragma omp for
        for (int i = 0; i < N; i++) {
            result_vector[i] = 0;
            for (int j = 0; j < M; j++) {
                result_vector[i] += matrix[i][j] * vector[j];
            }
        }
        end_time_parallel = omp_get_wtime();
    }
    result_time_parallel = (end_time_parallel - start_time_parallel);
    printf("parall total time: %f\n", result_time_parallel);
}
