/*
 * На процессе с номером 0 объявить и заполнить значениями матрицу a[8][8].
 * Создать новый тип данных для рассылки нечетных строк матрицы а в матрицу b[4][8],
 * и для рассылки четных строк матрицы в матрицу c[4][8], используя функцию MPI_Type_vector.
 */
#include <mpi.h>
#include <vector>
#include <cmath>
#define N 8
using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int tag = 1;
    int b[N / 2][N], c[N / 2][N];

    MPI_Datatype linesType;

    MPI_Type_vector(N / 2, N, N * 2, MPI_INT, &linesType);
    MPI_Type_commit(&linesType);

    if (rank == 0) {
        int a[N][N];
        printf("matrix a: \n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                a[i][j] = '0' + (3 * i + j) % 10;
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        for (int i = 0; i < 2; i++) {
            MPI_Send(&a[i][0], 1, linesType, i + 1, tag, MPI_COMM_WORLD);
        }
    }


    if (rank == 1) {
        MPI_Status status;
        int size;

        MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &size);

        int *tmpArray = new int[size];
        MPI_Recv(tmpArray, size, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        int count = 0;
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N; j++) {
                b[i][j] = tmpArray[count];
                count++;
            }
        }

        printf("matrix b: \n");
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    if (rank == 2) {

        MPI_Status status;
        int k;

        MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &k);

        int *tmpArray = (int *)malloc(sizeof(int) * k);
        MPI_Recv(tmpArray, k, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        int count = 0;
        printf("matrix c: \n");
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N; j++) {
                c[i][j] = tmpArray[count];
                count++;
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}