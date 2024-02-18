#include "realization.hpp"

#include <iostream>
#include <stdlib.h>

void GuideToUse() {
    std::cout << "Hello user, there are list of commands you can use\n";
    std::cout << "1 - get first function result\n";
    std::cout << "2 - get second function result\n";
}

int main() {
    int cmd = 0;

    GuideToUse();
    while(std::cin >> cmd) {
        switch (cmd) {
        case 1:
            int A, B;
            std::cin >> A >> B;
            std::cout << "The value of PrimeCount is: " << PrimeCount(A, B) << '\n';
            break;
        case 2:
            float C, D;
            std::cin >> C >> D;
            std::cout << "Square of figure: " << Square(C, D) << std::endl;
            break;
        default:
            std::cout << "There are no commands with that id\n";
            break;
        }
    }

    return 0;
}