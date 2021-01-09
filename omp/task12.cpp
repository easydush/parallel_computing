/* Модифицируйте задачу 1 так, что бы потоки распечатывали свои идентификаторы в
обратном порядке. Существует как минимум 5 способов решения. Постарайтесь найти как
можно больше.
   Первый способ
 */

#include <cstdio>
#include <omp.h>

int main()
{
    int threads = 8;
    omp_set_num_threads(threads);

#pragma omp parallel shared(threads)
    {
        for (;;)
        {
            if (omp_get_thread_num() + 1 == threads)
            {
                printf("Thread №%d, total: %d\n", omp_get_thread_num(),
                       omp_get_num_threads());
                threads--;
                break;
            }
        }

    }
}


