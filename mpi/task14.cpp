/*
 * Написать программу умножения матрицы на вектор z=Ax. Распределить матрицу А равномерно вдоль строк,
 * вектор х передать всем процессам. После выполнения операции умножения матрицы на вектор
 * на каждом процессе необходимо выполнить сбор полученных локальных частей результирующего вектора
 * на один из процессов. Использовать функцию MPI_Bcast для рассылки векторам,
 * для распределения элементов матрицы использовать функцию MPI_Scatter,
 * для сбора локальных частей результирующего вектора в глобальный вектор использовать функцию MPI_Gather.
 */
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define N 3
#define M 3

template <int rows, int cols>
void printMatrix(int (&matrix)[rows][cols])
{
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

template <int rows, int cols>
void fillMatrix(int (&matrix)[rows][cols])
{
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            matrix[i][j] = std::rand() % 9 + 1;
        }
    }
}

int main(int argc, char** argv) {
    int rank, word_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &word_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int x[M];
    int block = N / word_size;
    int local_a[5][M];

    if (rank == 0) {

        for (int i = 0; i < M; ++i) x[i] = rand() % 10;

        int a[N][M];

        fillMatrix(a);
        printf("matrix a\n");
        printMatrix(a);

        printf("vector x: ");

        for (int i = 0; i < M; ++i) {
           printf("%d ",x[i]);
        }
        printf("\n");

        int result[N];
        fill(result, result + N, 0);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                result[i] += a[i][j] * x[j];

        printf("result: ");

        for (int i = 0; i < N; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");

        MPI_Bcast(x, N, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&(a[0][0]), block * M, MPI_INT, &(local_a[0][0]), block * M, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Bcast(x, N, MPI_INT, 0, MPI_COMM_WORLD);
        int d[1];
        MPI_Scatter(&(d[0]), block * M, MPI_INT, &(local_a[0][0]), block * M, MPI_INT, 0, MPI_COMM_WORLD);
        int* res = new int[block];
        for (int i = 0; i < block; ++i) {
            res[i] = 0;
            for (int j = 0; j < M; ++j) {
                res[i] += local_a[i][j] * x[j];
            }
        }
    }
    finish: MPI_Finalize();
}
