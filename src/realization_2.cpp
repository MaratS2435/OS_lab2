#include "realization.hpp"

int PrimeCount(int A, int B) {
    int limit = B - A + 1; // размер отрезка
    int isPrime[limit];// массив для хранения простоты чисел на отрезке
    int count = 0; // счетчик простых чисел

    for (int i = 0; i < limit; ++i)
        isPrime[i] = 1;

    // Заполняем массив простыми числами с использованием решета Эратосфена
    for (int p = 2; p * p <= B; ++p) {
        int start = (A / p) * p; // начальное значение для простого числа p
        if (start < A) start += p; // если начальное значение меньше A, сдвигаем его на p
        for (int i = start > p*p ? start : p*p ; i <= B; i += p) {
            isPrime[i - A] = 0; // помечаем числа, кратные p, как составные
        }
    }

    // Считаем количество простых чисел на отрезке
    for (int i = 0; i < limit; ++i) {
        if (isPrime[i] == 1 && (A + i) > 1) { // проверяем, является ли число простым
            ++count;
        }
    }

    return count;
}

float Square(float A, float B) {
    return (A * B) / 2;
}