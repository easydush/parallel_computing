/*
 * Написатьпрограмму,вычисляющую элементы вектора z по формуле. Векторы х,у задаются на нулевом процессе
 * и равными блоками xyzi=αi+βi пересылаются остальным процессам,
 * a,b - заданные числа. Пересылка данных, осуществляется функцией MPI_Send.
 * Все процессы по формуле вычисляют свои элементы массива z.
 * Каждый процесс отправляет на нулевой процесс подсчитанные элементы.
 */
#include <cstdlib>
#include "mpi.h"
#include "vector"

#define SIZE 10
using namespace std;

void printVector(vector<int> x) {
    for (int i = 0; i < x.size(); i++) {
        printf("%d ", x[i]);
    };
    printf("\n");
}


vector<int> addVector(vector<int> &x, vector<int> &y) {
    vector<int> result(x.size());
    for (int i = 0; i < x.size(); i++) {
        result[i] = x[i] + y[i];
    }
    return result;
}

int main(int argc, char **argv) {
    int rank, num_threads;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
    int partSize = SIZE / (num_threads - 1);
    if (rank == 0) {
        vector<int> x(SIZE), y(SIZE);
        for (int i = 0; i < SIZE; i++) {
            x[i] = rand() % SIZE;
            y[i] = rand() % SIZE;
        }
        printVector(x);
        printVector(y);

        for (int i = 1; i < num_threads; i++) {
            vector<int> xPart(&x[partSize * (i - 1)], &x[partSize * i]),
                    yPart(&y[partSize * (i - 1)], &y[partSize * i]), result(partSize), auxResult(partSize);

            MPI_Send(&xPart[0], xPart.size(), MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&yPart[0], yPart.size(), MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Recv(&result[0], partSize, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("partly:\n");
            printVector(result);

        }
    } else {
        vector<int> xPart(partSize), yPart(partSize), result(partSize);
        MPI_Recv(&xPart[0], partSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&yPart[0], partSize, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        result = addVector(xPart, yPart);

        MPI_Send(&result[0], result.size(), MPI_INT, 0, 10, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}