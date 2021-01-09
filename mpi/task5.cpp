/*
 * Написать программу и запустить ее на p(=2,3,4,6,8) процессах.
 * На нулевом процессе объявить и задать массив из 12 элементов.
 * С помощью функции MPI_Send разослать блоки массива на остальные процессы.
 * Размер блока массива (12/p)+1.В результате на нулевом процессе должны быть элементы массивас 0 до 12/p,
 * на первом процессе с 12/p+1 до 2×(12/p),на 3м процессес 2×(12/p)+1 до 3×(12/p) и т.д.
 * Вывести элементы массива на экран на каждом процессе.
 * Обратите внимание, что не на все процессы хватит элементоd.
 * */
#include <iostream>
#include <cmath>
#include <mpi.h>
#include <stdio.h>
#define SIZE 12
using namespace std;

int *cut_array(int size, int process, int source[]) {
    int *result = new int[size];
    int count = 0;
    int head = (process - 1) * size;
    int tail = (process) * size;
    for (int i = head; i < tail; i++) {
        result[count] = source[i];
        count++;
    }
    return result;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, total_processes;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
    int messageSize;
    if (rank == 0) {
        int array[SIZE];
        for (int i = 0; i < SIZE; i++)
        {
            array[i] = rand() % SIZE;
            printf("%d ", array[i]);
        };
        printf("\n");

        for (int i = 1; i < total_processes; i++) {
            int size = SIZE / (total_processes + 1);
            int *message = cut_array(size, i, array);
            MPI_Send(message, size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &messageSize);
        int *array = new int[messageSize];
        MPI_Recv(array, messageSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("process #%d; messageSize = %d: \n", rank, messageSize);
        for (int i = 0; i < messageSize; i++) {
            printf("%d ",array[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;

}