/* Написать программу, в которой объявить и присвоить начальные значения элементам
двумерного массива d[6][8], для инициализации значений использовать генератор случайных
чисел. Используя конструкцию директивы omp parallel for и omp critical определить
минимальное и максимальное значения элементов двумерного массива. Количество нитей
задать самостоятельно. Результат выдать на экран. */

#include <cstdio>
#include <ctime>
#include <climits>
#include <omp.h>
#include <cstdlib>

int main()
{
    srand(time(nullptr));
    int d[6][8];

    // fill array with random numbers from 0 to 50
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            d[i][j] = rand() % 50;
            printf("%d, ", d[i][j]);
        }
        printf("\n");
    }

    int min = INT_MAX;
    int max = INT_MIN;
#pragma omp parallel for
    for (int i = 0; i < 6; i++)
    {
        printf("Thread %d is counting min and max\n", omp_get_thread_num());
        for (int j = 0; j < 8; j++)
        {
#pragma omp critical(minValue)
            {
                if (d[i][j] < min)
                    min = d[i][j];
            }
#pragma omp critical(maxValue)
            {
                if (d[i][j] > max)
                    max = d[i][j];
            }
        }
    }
    printf("MIN: %d, MAX: %d\n", min, max);
}