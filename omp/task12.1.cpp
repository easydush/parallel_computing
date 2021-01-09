#include <omp.h>
#include <zconf.h>
#include <cstdio>

/* Модифицируйте задачу 1 так, что бы потоки распечатывали свои идентификаторы в
обратном порядке. Существует как минимум 5 способов решения. Постарайтесь найти как
можно больше.
   Второй способ
 */

int main() {
    int nthreads, tid;

#pragma omp parallel num_threads(8) private(tid)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();
        sleep(nthreads - tid);
        printf("ping from thread %d of %d\n", tid, nthreads);
    }
}
