/* Написать программу, в которой определить две параллельные области, каждая из которых
содержит итерационную конструкцию for выполняющую инициализацию элементов
одномерных массивов целых чисел a[12], b[12] и c[12]. Число нитей перед первой областью
задать равным 3, перед второй – равным 4. Первая параллельная область выполняет
инициализацию элементов массивов a и b с использованием статического распределения
итераций, размер порции итераций выбрать самостоятельно, вторая параллельная область
выполняет инициализацию элементов массива c по следующему правилу c[i] = a[i] + b[i], с
использованием динамического распределения итераций, размер порции итераций выбрать
самостоятельно. В каждой области определить и выдать на экран количество нитей, номер
нити и результат выполнения цикла. Убедиться в правильности работы программы. */

#include <cstdio>
#include <omp.h>
#include <ctime>
#include <cstdlib>

#define N 12

int main() {
    srand(time(NULL));
    int a[N], b[N], c[N];

#pragma omp parallel num_threads(3)
    {
        printf("First area threads num: %d\n", omp_get_num_threads());
#pragma omp for schedule(static, 4)
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 20;
            b[i] = rand() % 20;

            printf("a[i]: %d, b[i]: %d, thread number %d\n", a[i], b[i], omp_get_thread_num());
        }
    }

#pragma omp parallel num_threads(4)
    {
        printf("Second area threads num: %d\n", omp_get_num_threads());
#pragma omp for schedule(dynamic, 4)
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
            printf("c[i]: %d, thread number  %d\n", a[i], omp_get_thread_num());

        }
    }

}
