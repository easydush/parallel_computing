/*
 * Написать программу вычисления поэлементного умножения матриц. BСi,j=Ai,ji.j
 * Использовать функции MPI_Scatter для распределения элементов матриц A и B,
 * и MPI_Gather для сбора вычисленных данных в матрицу C.
 */
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#define N 3

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

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int block = N * N / size;
    int *loc_a = new int[block];
    int *loc_b = new int[block];

    if (rank == 0) {

        int a[N][N], b[N][N];

        fillMatrix(a);
        printf("matrix a \n");
        printMatrix(a);
        fillMatrix(b);
        printf("matrix b \n");
        printMatrix(b);

        MPI_Scatter(&(a[0][0]), block, MPI_INT, &loc_a[0], block, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&(b[0][0]), block, MPI_INT, &loc_b[0], block, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < block; ++i) {
            loc_a[i] *= loc_b[i];
        }

        MPI_Gather(&loc_a[0], block, MPI_INT, &(a[0][0]), block, MPI_INT, 0, MPI_COMM_WORLD);

        printf("matrix c \n");
        printMatrix(a);

    } else {
        int res[1];

        MPI_Scatter(&res[1], block, MPI_INT, &loc_a[0], block, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&res[1], block, MPI_INT, &loc_b[0], block, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < block; ++i) {
            loc_a[i] *= loc_b[i];
        }
        MPI_Gather(&loc_a[0], block, MPI_INT, &res[1], block, MPI_INT, 0, MPI_COMM_WORLD);
    }
    finish: MPI_Finalize();
}
