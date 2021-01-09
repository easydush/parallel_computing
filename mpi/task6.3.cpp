/*
 * На одном из процессов объявить и заполнить значениями верхнюю треугольную матрицу.
 * Создать новый тип данных для передачи ненулевых элементов матрицы, используя конструктор MPI_Type_indexed.
 * Выполнить передачу элементов на все процессы с помощью функции MPI_Bcast.
 */
#include <iostream>
#include <mpi.h>

using namespace std;

template<int rows, int cols>
void printMatrix(int (&matrix)[rows][cols]) {
    printf("\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t matrixSize = 4;

    int matrix[matrixSize][matrixSize];
    for (auto row = 0; row < matrixSize; ++row)
        for (auto col = 0; col < matrixSize; ++col)
            matrix[row][col] = 0;

    int blockLengths[matrixSize];
    int indices[matrixSize];
    for (auto i = 0; i < matrixSize; ++i) {
        blockLengths[i] = matrixSize - i;
        indices[i] = matrixSize * i + i;
    }

    MPI_Datatype matrixType;
    MPI_Type_indexed(matrixSize, blockLengths, indices, MPI_INT, &matrixType);
    MPI_Type_commit(&matrixType);

    int processNumber;
    int processesCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
    MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

    if (processNumber == 0) {
        for (auto row = 0; row < matrixSize; ++row)
            for (auto col = 0; col < matrixSize; ++col)
                matrix[row][col] = rand() % 10;
        printf("\n#%d Source: \n", rank);
        printMatrix(matrix);
    }

    MPI_Bcast(&matrix, 1, matrixType, 0, MPI_COMM_WORLD);

    if (processNumber != 0) {
        printf("\n#%d Received: \n", rank);
        printMatrix(matrix);
    }
    MPI_Type_free(&matrixType);
    MPI_Finalize();
    return 0;
}