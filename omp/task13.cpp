/* На нулевом процессе задан массив целых чисел, который является двоичным представлением
десятичного числа. Написать параллельную программу вычисления десятичного числа из
заданного двоичного представления.
int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
Число x может быть представлено в системе с основанием p, как x=an*pn+an-1*pn-1+
a1*p1+a0*p0, где an ... a0 - цифры в представлении данного числа. */

#include <cstdio>
#include <omp.h>
#include <cmath>


int main() {
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int x = 0;

#pragma omp parallel for reduction(+:x)
    for (int i = 0; i < 30; i++) {
        x += a[30 - 1 - i] * pow(2, i);
        printf("working %d, for %d counted %d\n", omp_get_thread_num(),30 - 1 - i, x);
    }
    printf("Answer %d", x);
}