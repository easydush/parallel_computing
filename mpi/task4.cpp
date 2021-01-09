/*
 * Используя блокирующую операцию передачи сообщений (MPI_Send и MPI_Recv) выполнить
 * пересылку данных одномерного массива,из процесса с номером 1 на остальные процессы группы.
 * На процессах получателях до выставления функции приема (MPI_Recv) определить количество принятых данных(MPI_Probe).
 * Выделить память под размер приемного буфера,после чего вызвать функцию MPI_Recv.
 * Полученное сообщение выдать на экран.
 */
#include "iostream"
#include "mpi.h"
#include <ctime>
#include <cstdlib>

#define SIZE 10

int main(int argc, char **argv) {
    int rank, size, bufElems;
    srand(time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 1) {
        int arr[SIZE];
        // filling array with random values
        for (int i = 0; i < SIZE; i++) {
            arr[i] = rand() % SIZE;
        }
        // sending array to other threads
        for (int i = 0; i < size; i++) {
            if (i != rank) {
                MPI_Send(arr, SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        // checking incoming message's length
        MPI_Status status;
        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &bufElems);
        // setting buffer memory depends on ,essage length
        int *buf = (int *) malloc(sizeof(int) * bufElems);
        // getting an array
        MPI_Recv(buf, bufElems, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < bufElems; i++) {
            printf("%d ", buf[i]);
        };
        printf("\n Process number: %d\n", rank);
        free(buf);
    }
    MPI_Finalize();
    return 0;
}