/*
 * Написать программу вычисления скалярного произведения двух векторов (x,)y(y=∑n−1i=0xii.
 * Для распределения элементов вектора x по процессам использовать функцию MPI_Scatter.
 * Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum.
 */
#include <mpi.h>
#include <math.h>
#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int block = 3;
    int N = size * block;
    int* x = new int[N];
    int* x1 = new int[block];
    int* y = new int[N];
    int* y1 = new int[N];

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            x[i] = rand() % 100;
            y[i] = rand() % 10;
        }

    }
    MPI_Scatter(&x[0], block, MPI_INT, &x1[0], block, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&y[0], block, MPI_INT, &y1[0], block, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("x: ");
        for (int i = 0; i < N; ++i){
            printf("%d ",x[i]);
        }
        printf("\ny: ");
        for (int i = 0; i < N; ++i) {
            printf("%d ",y[i]);
        }
        printf("\n");
    }

    int scalar = 0;
    for (int i = 0; i < block; ++i) {
        scalar += x1[i] * y1[i];
    }

    int ans;
    MPI_Reduce(&scalar, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) printf("Answer is = %d \n",ans);

    finish: MPI_Finalize();
}