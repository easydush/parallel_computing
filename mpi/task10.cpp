/*
 * Написать программу вычисления нормы |x||bs(x)|1=∑n−1i=0 вектора x.
 * Для распределения элементов вектора x по процессам использовать функцию MPI_Scatter.
 * Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum.
 */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int block = 3;
    int N = size * block;
    int* x = new int[N];
    int* y = new int[block];

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            x[i] = rand() % 10;
        }
    }

    MPI_Scatter(&x[0], block, MPI_INT, &y[0], block, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("x: ");
        for (int i = 0; i < N; ++i){
            printf("%d ",x[i]);
        }
        printf("\n");
    }
    int sum = 0;
    for (int i = 0; i < block; ++i) {
        sum += abs(y[i]);
    }
    int answer;
    MPI_Reduce(&sum, &answer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Answer is %d", answer);
    }
    finish: MPI_Finalize();
}