/*Написать программу, в которой, используя возможности OpenMP найти все простые числа в
заданном с клавиатуры диапазоне. Количество потоков определить самостоятельно.
Результат выдать на экран.*/


#include <cstdio>
#include <omp.h>

bool is_prime(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main() {
    int low;
    int high;

    printf("Enter the first value :\n");
    scanf("%d", &low);

    printf("Enter the second value :\n");
    scanf("%d", &high);


#pragma omp parallel for ordered schedule(dynamic) num_threads(4)
    for (int i = low; i <= high; i++) {
        bool prime = is_prime(i);
        if (prime) {
#pragma omp ordered
            printf("Prime number %d\n", i);
        }
        printf("Thread %d\n Value: %d,\n Prime: %d\n", omp_get_thread_num(), i, prime);
    }

}