/*
 * Пусть на нулевом процессе определены и заданы массив из десяти целых чисел и массив из восьми вещественных чисел.
 * Упаковать два массива. Передать упакованные данные на все процессы.
 * Распаковать на всех процессах принятые данные. Результат выдать на экран.
 */
#include <iostream>
#include <mpi.h>

using namespace std;

template<typename TNumber>
void printMatrix(
        const string &title,
        TNumber array[],
        size_t arrayLength) {
    cout << title << endl;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("process #%d \t", rank);
    for (auto i = 0; i < arrayLength; ++i)
        cout << " " << array[i];
    cout << endl;
}

struct TwoArrays {
    int intArray[10];
    double doubleArray[8];
};

int next_int(int arg) {
    return rand() % (arg + 1);
}

double next_double(int arg) {
    return rand() % (arg + 1) * 0.1;
}

int main(int argc, char **argv) {
    MPI_Init(nullptr, nullptr);

    int blockLengths[2] = {10, 8};
    MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};
    MPI_Aint adr[2] =
            {
                    offsetof(struct TwoArrays, intArray),
                    offsetof(struct TwoArrays, doubleArray)
            };

    MPI_Datatype arraysType;
    MPI_Type_create_struct(2, blockLengths, adr, types, &arraysType);
    MPI_Type_commit(&arraysType);

    int rank;
    int processesCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

    TwoArrays arrays;
    if (rank == 0) {
        for (auto i = 0; i < 10; ++i)
            arrays.intArray[i] = next_int(i);
        printMatrix("Int array", arrays.intArray, 8);
        for (auto i = 0; i < 8; ++i)
            arrays.doubleArray[i] = next_double(i);
        printMatrix("Double array", arrays.doubleArray, 8);
    }

    MPI_Bcast(&arrays, 1, arraysType, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        printMatrix("Int array", arrays.intArray, 8);
        printMatrix("Double array", arrays.doubleArray, 8);
    }

    MPI_Type_free(&arraysType);
    MPI_Finalize();
    return 0;
}