/*
 * Написать программу вычисления нормы матрицы А по формуле,где m - число столбцов, n - число строк.
 * Для распределения (bs(a[i][j])) s = max j = 0 ..m∑ni=0
 * a элементов матрицы А по процессам использовать функцию MPI_Scatter.
 * Для получения итогового значения использовать функцию MPI_Reduce с операцией MPI_MAX.
 */
#include <stdio.h>
#include "cstdlib"
#include "mpi.h"

#define N 4

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
            matrix[i][j] = std::rand() % 8 + 1;
        }
    }
}

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    int i, rank, size, globalres;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a[N][N];
    fillMatrix(a);
    if (rank == 0)
        printMatrix(a);

    int aa[N];

    MPI_Scatter(a, N * N / size, MPI_INT, aa, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (i = 0; i < N; i++)
        sum += abs(aa[i]);

    MPI_Reduce(&sum, &globalres, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Finalize();

    if (rank == 0)
        printf("answer is: %d \n", globalres);
}
