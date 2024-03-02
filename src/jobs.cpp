#include "jobs.h"

const std::map<std::string, jobFunc> functionMap = {
        {"AAA", AAA},
        {"BBB", BBB},
        {"CCC", CCC},
        {"DDD", DDD},
        {"EEE", EEE},
        {"FFF", FFF},
        {"GGG", GGG},
        {"HHH", HHH}
    };
void AAA() {
    std::cout << "Привет, я джоб AAA!" << std::endl;
    std::cout << "Я работаю!" << std::endl;
}

void BBB() {
    std::cout << "Привет, я джоб BBB!" << std::endl;
    int number;
    std::cout << "Введите число: ";
    std::cin >> number;
    std::cout << "Вы ввели число: " << number << std::endl;
}

void CCC() {
    std::cout << "Привет, я джоб CCC!" << std::endl;
    srand(time(NULL));
    int randomNum = rand() % 10 + 1;
    std::cout << "Случайное число: " << randomNum << std::endl;
}

void DDD() {
    std::cout << "Привет, я джоб DDD!" << std::endl;
    //throw std::runtime_error("Ошибка в джобе DDD");
}

void EEE() {
    std::cout << "Привет, я джоб EEE!" << std::endl;
    double num1, num2;
    std::cout << "Введите первое число: ";
    std::cin >> num1;
    std::cout << "Введите второе число: ";
    std::cin >> num2;
    
    if (num2 == 0) {
        throw std::runtime_error("Ошибка: деление на ноль!");
    }
    
    double result = num1 / num2;
    std::cout << "Результат деления: " << result << std::endl;
}

void FFF() {
    std::cout << "Привет, я джоб FFF!" << std::endl;
    std::string input;
    std::cout << "Введите строку: ";
    std::cin >> input;
    
    std::string reversed;
    for (int i = input.length() - 1; i >= 0; i--) {
        reversed += input[i];
    }
    
    std::cout << "Строка в обратном порядке: " << reversed << std::endl;
}

void GGG() {
    std::cout << "Привет, я джоб GGG!" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << "Привет, мир!" << std::endl;
    }
}

void HHH() {
    std::cout << "Привет, я джоб HHH!" << std::endl;
    int number;
    std::cout << "Введите число: ";
    std::cin >> number;
    
    int factorial = 1;
    for (int i = 1; i <= number; i++) {
        factorial *= i;
    }
    
    std::cout << "Факториал числа " << number << " равен " << factorial << std::endl;
}