/*
 * Пусть векторы х,у заданы на нулевом процессе. Написать программу, в которой векторы равными блоками с нулевого
 * процесса пересылаются остальным процессам. Пересылка данных осуществляется функцией MPI_Send.
 * Все процессы по формуле вычисляют свои элементы искомых массивов. Каждый процесс отправляет на нулевой процесс
 * подсчитанные элементы массивов. В программе реализовать следующие операции вычисления векторов:
 * ●, где - заданные числа;
 * xyzi=αi+βi,βα●;
 * zi=xi*yi●
 * Обмен элементов двух векторов .
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, word_size;
    MPI_Status status;
    int a = 1, b = 10;
    const int block = 3;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &word_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int N = 12;

    if (rank == 0) {
        int x[N];
        int y[N];
        int z1[N];
        int z2[N];
        for (int i = 0; i < N; ++i) {
            x[i] = rand() % 20;
            y[i] = rand() % 10;
        }
        printf("\nx: ");
        for (int i = 0; i < N; ++i) {
            printf("%d ", x[i]);
        }
        printf("\ny: ");
        for (int i = 0; i < N; ++i) {
            printf("%d ", y[i]);
        }
        printf("\n");
        for (int i = 1; i < word_size; i++) {
            MPI_Send(&x[(i - 1) * block], block, MPI_INT, i, 10, MPI_COMM_WORLD);
            MPI_Send(&y[(i - 1) * block], block, MPI_INT, i, 10, MPI_COMM_WORLD);
        }

        for (int i = 1; i < word_size; i++) {
            MPI_Recv(&z1[(i - 1) * block], block, MPI_INT, i, 100, MPI_COMM_WORLD, &status);

            MPI_Recv(&z2[(i - 1) * block], block, MPI_INT, i, 1000, MPI_COMM_WORLD, &status);

            MPI_Recv(&x[(i - 1) * block], block, MPI_INT, i, 2000, MPI_COMM_WORLD, &status);
            MPI_Recv(&y[(i - 1) * block], block, MPI_INT, i, 2000, MPI_COMM_WORLD, &status);
        }

        printf("z1: ");
        for (int i = 0; i < 12; ++i) printf(" %d  ", z1[i]);

        printf("\nz2: ");
        for (int i = 0; i < 12; ++i) printf(" %d  ", z2[i]);

        printf("\nnew x: ");
        for (int i = 0; i < 12; ++i) printf(" %d  ", x[i]);

        printf("\nnew y: ");
        for (int i = 0; i < 12; ++i) printf(" %d  ", y[i]);
        printf("\n");
    }
    else {
        int x[block];
        int y[block];
        int z[block];
        MPI_Recv(&x, block, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&y, block, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        //task A
        for (int i = 0; i < block; ++i) {
            z[i] = a * x[i] + b * y[i];
        }

        MPI_Send(&z, block, MPI_INT, 0, 100, MPI_COMM_WORLD);

        //task B
        for (int i = 0; i < block; i++) {
            z[i] = x[i] * y[i];
        }

        MPI_Send(&z, block, MPI_INT, 0, 1000, MPI_COMM_WORLD);

        //task C
        MPI_Send(&y, block, MPI_INT, 0, 2000, MPI_COMM_WORLD);
        MPI_Send(&x, block, MPI_INT, 0, 2000, MPI_COMM_WORLD);

    }
    finish:MPI_Finalize();
}