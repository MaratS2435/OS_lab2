#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <malloc.h>

typedef enum {
    FIRST,
    SECOND,
} Contract;

Contract current = FIRST;

const char* lib_name1 = "./lib_first.so";
const char* lib_name2 = "./lib_second.so";

int (*PrimeCount)(int A, int B) = NULL;
float (*Square)(float A, float B) = NULL;

void* libHandle = NULL;

void LoadDynamicLib(Contract con) {
    if (con == FIRST) {
        libHandle = dlopen(lib_name1, RTLD_LAZY);
    } else if (con == SECOND) {
        libHandle = dlopen(lib_name2, RTLD_LAZY);
    } else {
        std::cerr << "Contract if error\n";
        exit(EXIT_FAILURE);
    }

    if (!libHandle) {
        std::cerr << dlerror();
        exit(EXIT_FAILURE);
    }
}

void LoadContract() {
    LoadDynamicLib(current);
    PrimeCount = (int (*)(int, int))dlsym(libHandle, "PrimeCount");
    Square = (float (*)(float, float))dlsym(libHandle, "Square");
 
    if (PrimeCount == NULL || Square == NULL) {
        std::cerr << "dlsym error\n";
    }
}

void UnloadDynamicLib() {
    dlclose(libHandle);
}

void ChangeContract() {
    UnloadDynamicLib();

    if (current == FIRST) {
        current = SECOND;
    } else {
        current = FIRST;
    }

    LoadContract();
}

void GuideToUse() {
    std::cout << "Hello user, there are list of commands you can use\n";
    std::cout << "0 - change current contract\n";
    std::cout << "1 - get first function result\n";
    std::cout << "2 - get second function result\n";
}

int main() {
    LoadContract();

    int cmd = 0;

    GuideToUse();
    while(std::cin >> cmd) {
        switch (cmd) {
            case 0:
               ChangeContract(); 
               if (current == FIRST) {
                std::cout << "Contract was changed to first\n";
               } else {
                std::cout << "Contract was changed to second\n";
               }
               break;
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
    UnloadDynamicLib();


    return 0;
}