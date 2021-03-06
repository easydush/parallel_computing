/* Написать программу, в которой, объявить и заполнить случайными значениями массив целых
чисел. Используя возможности OpenMP найти максимальное числовое значение элементов
массива, кратное 7. Длину массива и количество потоков определить самостоятельно.
Результат выдать на экран. Для синхронизации числовых значений максимума используется
механизм критических секций. */

#include <cstdio>
#include <ctime>
#include <omp.h>
#include <cstdlib>

int main()
{
    srand(time(NULL));
    int a[20];

    for (int i = 0; i < 20; i++)
        a[i] = rand() % 50;

    int max = 0;

#pragma omp parallel for shared(max)
    for (int i = 0; i < 20; i++)
    {
#pragma omp critical
        if (a[i] % 7 == 0 && a[i] > max)
        {
            max = a[i];
        }
    }

    printf("Max number mod 7 = 0: %d", max);
}
