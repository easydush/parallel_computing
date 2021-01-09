/*
 * Написать программу, запустить ее на 2х процессах. На нулевом процессе задать массива из 10 элементов,
 * значения сгенерировать случайным образом. Переслать этот массив целиком первому процессу с помощью функции MPI_Send.
 * Распечатать на первом процессе принятый массив.
 */
#include <iostream>
#include "mpi.h"
#include <ctime>
using namespace std;
int main(int argc, char **argv) {
#define RANDOM 10;
    srand(time(NULL));
    int i, size, rank;

    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        int a[10];
        // random array filling
        for (int i = 0; i < 10; i++)
            a[i] = rand() % RANDOM;
        for (int i = 1; i < size; i++) {
            MPI_Send(&a, 10, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    } else {
        int print[10];
        printf("Process number %d print array \n", rank);
        MPI_Recv(&print, 10, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        for (int i = 0; i < 10; i++) {
            printf("%d ", print[i]);
        }
    }
    MPI_Finalize();
}