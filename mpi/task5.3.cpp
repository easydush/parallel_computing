/*
 * Предположим, что на каждом процессе есть свое число х.
 * Необходимо найти сумму всех элементов х и сумму разослать всем процессам.
 * Можно использовать связку двух функций: MPI_Reduce и MPI_Bcast,
 * т.е. сначала результат операции редукции разместить на одном процессе,
 * а потом с помощью функции MPI_Bcast разослать его всем остальным процессам.
 * Можно использовать функцию MPI_Allreduce, которая аналогична функции MPI_Reduce,
 * но сохраняет результат редукции в адресном пространстве всех процессов.
 * Написать программу для измерения времени, необходимого для выполнения функции MPI_Allreduce на MPI_COMM_WORLD.
 * Как изменяется времяработы функции MPI_Allreduce при изменении размера коммуникатора?
 * Насколько быстрее работает MPI_Allreduce чем связка двух функций MPI_Reduce и MPI_Bcast?
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int x, sum = 0;
    double beginTimeStamp, endTimeStamp, funcExecutionDuration, global;
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    x = rand();
    sum = 0;

    beginTimeStamp = MPI_Wtime();


    MPI_Allreduce(&x, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    endTimeStamp = MPI_Wtime();
    funcExecutionDuration = endTimeStamp - beginTimeStamp;
    MPI_Reduce(&funcExecutionDuration, &global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("MPI_Allreduce: \t\t\t %f\n", global);
    }

    beginTimeStamp = MPI_Wtime();

    MPI_Reduce(&x, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    endTimeStamp = MPI_Wtime();
    funcExecutionDuration = endTimeStamp - beginTimeStamp;
    MPI_Reduce(&funcExecutionDuration, &global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Связка MPI_Reduce + MPI_Bcast: \t %f\n", global);
    }


    MPI_Finalize();
}