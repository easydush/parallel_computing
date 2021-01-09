/* Модифицируйте задачу 1 так, что бы потоки распечатывали свои идентификаторы в
обратном порядке. Существует как минимум 5 способов решения. Постарайтесь найти как
можно больше.
   Третий способ
 */
#include <cstdio>
#include <omp.h>
#include <stack>

int main() {
    int stack_size = 8;
    std::stack<int> order;
    omp_set_num_threads(stack_size);

#pragma omp parallel shared(order)
    {
#pragma omp critical
        {
            order.push(omp_get_thread_num());
            printf("pushed %d\n", omp_get_thread_num());
        }
    }
    for (int i = 0; i < stack_size; i++) {
        printf("ping from thread %d \n", order.top());
        order.pop();
    }
}