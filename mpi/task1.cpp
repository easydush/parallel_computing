//Написать mpi программу,печатающую "Hello,World!".
//Запустить программу на 4 процессах.

#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    printf("Hello world!\n");
    MPI_Finalize();
    return 0;
}