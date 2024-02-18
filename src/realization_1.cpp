#include "realization.hpp"

int PrimeCount(int A, int B) {
    int n = 0, f = 0;
    for (int i = A; i <= B; ++i) {
        for (int j = 2; j < i; ++j) {
            if (i % j == 0) {
                f = 1;
                break;
            }
        }
        if (f == 0 && i > 1) {
            n++;
        } else {
            f = 0;
        }
    }
    
    return n;
}

float Square(float A, float B) {
    return A*B;
}