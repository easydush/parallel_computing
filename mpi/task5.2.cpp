/*
 * Пусть A(n,m) – матрица, созданная на нулевом процессе.
 * Пусть есть 4 процесса и процесс 0 посылает части этой матрицы другим процессам.
 * Процессор 1 получает A(i,j) для i=n/2+1,...,n,иj=1,...,m/2.
 * Процессор 2 получает A(i,j) для i=1,...,n/2 и j=m/2+1,...,m и
 * Процессор 3 получает A(i,j) для i=n/2+1,...,n and j=m/2,...,m.
 * Это двумерная декомпозиция А на четыре процесса.
 * Написать программу рассылки частей матрицы по процессам, используя функцию MPI_Scatterv.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int charArray(char ***array, int n, int m) {
    char *p = (char *) malloc(n * m * sizeof(char));
    (*array) = (char **) malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
        (*array)[i] = &(p[i * m]);
    return 0;
}


int main(int argc, char **argv) {
    char **global, **local;
    const int gridsize = 4;
    const int procgridsize = 2;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // создание и вывод главной матрицы на нулевом процессе
        charArray(&global, gridsize, gridsize);
        for (int i = 0; i < gridsize; i++) {
            for (int j = 0; j < gridsize; j++)
                global[i][j] = '0' + (3 * i + j) % 10;
        }

        printf("Общая матрица:\n");
        for (int i = 0; i < gridsize; i++) {
            for (int j = 0; j < gridsize; j++){
                putchar(global[i][j]);
            }
            printf("\n");
        }
    }
    // выделение и отправка подмассивов

    charArray(&local, gridsize / procgridsize, gridsize / procgridsize);

    int sizes[2] = {gridsize, gridsize};
    int subsizes[2] = {gridsize / procgridsize, gridsize / procgridsize};
    int starts[2] = {0, 0};
    MPI_Datatype type, subarrtype;
    MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_CHAR, &type);
    MPI_Type_create_resized(type, 0, gridsize / procgridsize * sizeof(char), &subarrtype);
    MPI_Type_commit(&subarrtype);

    char *globalptr = NULL;
    if (rank == 0) globalptr = &(global[0][0]);

    int sendcounts[procgridsize * procgridsize];
    int displs[procgridsize * procgridsize];

    if (rank == 0) {
        for (int i = 0; i < procgridsize * procgridsize; i++) sendcounts[i] = 1;
        int disp = 0;
        for (int i = 0; i < procgridsize; i++) {
            for (int j = 0; j < procgridsize; j++) {
                displs[i * procgridsize + j] = disp;
                disp += 1;
            }
            disp += ((gridsize / procgridsize) - 1) * procgridsize;
        }
    }


    MPI_Scatterv(globalptr, sendcounts, displs, subarrtype, &(local[0][0]),
                 gridsize * gridsize / (procgridsize * procgridsize), MPI_CHAR,
                 0, MPI_COMM_WORLD);

    // Вывод подмассива на каждом процессе
    for (int p = 0; p < size; p++) {
        if (rank == p) {
            printf("процесс № %d:\n", rank);
            for (int i = 0; i < gridsize / procgridsize; i++) {
                for (int j = 0; j < gridsize / procgridsize; j++) {
                    putchar(local[i][j]);
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}