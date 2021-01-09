/*
 * Создать тип данных с помощью функции MPI_Type_create_struct и выполнить рассылку строк матрицы a[8][8]
 * на 4 процесса в матрицу d[2][8] по следующей схеме:
 *      на нулевой процесс строки 0 и 4,
 *      на первый процесс строки 1 и 5,
 *      на второй процесс строки 2 и 6,
 *      на третий процесс строки 3 и 7.
 */
#include <iostream>
#include <mpi.h>
using namespace std;

struct LinesType
{
    int line1[8];
    int line2[8];
};

int main(int argc, char **argv)
{
    int size, rank;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int tag = 1;
    struct LinesType linesType;
    const int n = 8;
    int blockLengths[2] = { n, n };

    MPI_Datatype types[2] = { MPI_INT, MPI_INT };
    MPI_Aint adr[2];

    adr[0] = offsetof(struct LinesType, line1);
    adr[1] = offsetof(struct LinesType, line2);

    MPI_Datatype arrType;

    MPI_Type_create_struct(2, blockLengths, adr, types, &arrType);
    MPI_Type_commit(&arrType);

    if (rank == 4) {
        int a[n][n];
        printf("matrix A: \n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = rand() % (10 + rank) + rank;
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < n; j++) {
                linesType.line1[j] = a[i][j];
                linesType.line2[j] = a[i + 4][j];
            }
            MPI_Send(&linesType, 2 * n, arrType, i, tag, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Status status;

        MPI_Recv(&linesType, 2 * n, arrType, 4, tag, MPI_COMM_WORLD, &status);
        int matrixD[2][n];
        for (int j = 0; j < n; j++) {
            matrixD[0][j] = linesType.line1[j];
            matrixD[1][j] = linesType.line2[j];
        }

        printf("received: \n");
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%d ", matrixD[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Type_free(&arrType);
    MPI_Finalize();
    return 0;
}
