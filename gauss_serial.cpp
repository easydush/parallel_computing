#include <omp.h>
#include <cstdlib>
#include <cstdio>

int main(int argc, char *argv[]) {
    int n = 2;
    double t = omp_get_wtime();
    double *a;
    a = static_cast<double *>(malloc(sizeof(*a) * n * n));
    double *b;
    b = static_cast<double *>(malloc(sizeof(*b) * n));
    double *x;
    x = static_cast<double *>(malloc(sizeof(*x) * n));
// Матрица коэффициентов
// Столбец свободных членов
// Неизвестные
    for (int i = 0; i < n; i++) {
// Инициализация
        srand(i * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * n + j] = rand() % 100 + 1;
        b[i] = rand() % 100 + 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%12.4f ", a[i * n + j]);
        printf(" | %12.4f\n", b[i]);
    }

    // Прямой ход -- O(n^3)
    for (int k = 0; k < n - 1; k++) {
// Исключение x_i из строк k+1...n-1
        double pivot = a[k * n + k];
        for (int i = k + 1; i < n; i++) {
// Из уравнения (строки) i вычитается уравнение k
            double lik = a[i * n + k] / pivot;
            for (int j = k; j < n; j++)
                a[i * n + j] -= lik * a[k * n + j];
            b[i] -= lik * b[k];
        }
    }
// Обратный ход -- O(n^2)
    for (int k = n - 1; k >= 0; k--) {
        x[k] = b[k];
        for (int i = k + 1; i < n; i++)
            x[k] -= a[k * n + i] * x[i];
        x[k] /= a[k * n + k];
    }
    free(b);
    free(a);
    t = omp_get_wtime() - t;
    printf("serial: n %d, time (msec) %.6f\n", n, 1000 * t);

    printf("X[%d]:", n);
    for (int i = 0; i < n; i++)
        printf("%f ", x[i]);
    printf("\n");

    free(x);
    return 0;
}