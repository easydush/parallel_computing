/* Написать программу, в которой объявлен массив из 16000 элементов и инициализирован так, что значение элемента массива
 * равно его порядковому номеру. Затем создайте результирующий массив, в котором (за исключением крайних элементов)
 * будут средние значения исходного массива:
 * b[i] = (a[i-1] + a[i] + a[i+1])/3.0
Запустите программу с 2,4, и 8-ю процессами при различных типах распределения работ
 (static, dynamic, guided, auto(runtime, если auto не работает)) и посмотреть время на разных размерах порций.
 Пояснить различие времени выполнения.
*/
#include <iostream>
#include <cstdio>
#include <omp.h>

#define N 16000

int test(int T) {
    // инициализация массивов и заполнение массива а
    int a[N];
    double b[N];
    for (int i = 0; i < N; i++) {
        a[i] = i + 1;
    }

    // заполнение массива b dynamic
    double start_time = omp_get_wtime();

#pragma omp parallel for schedule(tea) num_threads(T)
    for (int i = 0; i < N - 1; i++) {
        if (i == 0) {
            b[i] = (b[i] + b[i + 1]) / 2.0;
        } else if (i == N - 1) {
            b[i] = (b[i] + b[i - 1]) / 2.0;
        } else {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    double end_time = omp_get_wtime();
    double result_time = (end_time - start_time);
    printf("total time for dynamic:  %f sec with %d threads\n", result_time, T);

    // заполнение массива b static
    start_time = omp_get_wtime();
#pragma omp parallel for schedule(static) num_threads(T)
    for (int i = 0; i < N - 1; i++) {
        if (i == 0) {
            b[i] = (b[i] + b[i + 1]) / 2.0;
        } else if (i == N - 1) {
            b[i] = (b[i] + b[i - 1]) / 2.0;
        } else {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    end_time = omp_get_wtime();
    result_time = (end_time - start_time);
    printf("total time for static: \t %f sec with %d threads\n", result_time, T);

    start_time = omp_get_wtime();
    // заполнение массива b guided
#pragma omp parallel for schedule(guided) num_threads(T)
    for (int i = 0; i < N - 1; i++) {
        if (i == 0) {
            b[i] = (b[i] + b[i + 1]) / 2.0;
        } else if (i == N - 1) {
            b[i] = (b[i] + b[i - 1]) / 2.0;
        } else {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    end_time = omp_get_wtime();
    result_time = (end_time - start_time);
    printf("total time for guided: \t %f sec with %d threads\n", result_time, T);

    start_time = omp_get_wtime();

    // заполнение массива b auto
#pragma omp parallel for schedule(auto) num_threads(T)
    for (int i = 0; i < N - 1; i++) {
        if (i == 0) {
            b[i] = (b[i] + b[i + 1]) / 2.0;
        } else if (i == N - 1) {
            b[i] = (b[i] + b[i - 1]) / 2.0;
        } else {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    end_time = omp_get_wtime();
    result_time = (end_time - start_time);
    printf("total time for auto: \t %f sec with %d threads\n", result_time, T);
}

int main() {
    // цикл для смены количества потоков
    int times[3] = {2, 4, 8};
    for (int i = 0; i < 3; i++) {
        test(times[i]);
        printf("\n");
    }
}
