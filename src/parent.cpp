#include "shared_memory.hpp"

int main() {
    int temp;
    std::string fileName1, fileName2;
    std::cin >> fileName1 >> fileName2;
    const char * name1 = fileName1.c_str();
    const char * name2 = fileName2.c_str();

    int shared_memory_fd1 = shm_open(shared_memory_name1, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (shared_memory_fd1 == -1) {
        std::cerr << "Ошибка при открытии shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Изменяю размер shared_memory до размера структуры
    if (ftruncate(shared_memory_fd1, sizeof(SharedMemory)) == -1) {
        std::cerr << "Ошибка при изменении размкера shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    int shared_memory_fd2 = shm_open(shared_memory_name2, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (shared_memory_fd2 == -1) {
        std::cerr << "Ошибка при открытии shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Изменяю размер shared_memory до размера структуры
    if (ftruncate(shared_memory_fd2, sizeof(SharedMemory)) == -1) {
        std::cerr << "Ошибка при изменении размкера shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Отображаю участок памяти
    SharedMemory* shared_memory1 = (SharedMemory*)mmap(NULL, sizeof(SharedMemory), prots, flags, shared_memory_fd1, 0);
    if (shared_memory1 == MAP_FAILED) {
        std::cerr << "Ошибка при отображении shared_memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Отображаю участок памяти
    SharedMemory* shared_memory2 = (SharedMemory*)mmap(NULL, sizeof(SharedMemory), prots, flags, shared_memory_fd2, 0);
    if (shared_memory2 == MAP_FAILED) {
        std::cerr << "Ошибка при отображении shared_memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Инициализация семафоров
    if (sem_init(&shared_memory1->semaphore1, 1, 0) == -1) {
        std::cerr << "Ошибка при инициализации семафора 1" << '\n';
        exit(EXIT_FAILURE);
    }

    if (sem_init(&shared_memory2->semaphore1, 1, 0) == -1) {
        std::cerr << "Ошибка при инициализации семафора 2" << '\n';
        exit(EXIT_FAILURE);
    }

    if (sem_init(&shared_memory1->semaphore2, 1, 0) == -1) {
        std::cerr << "Ошибка при инициализации семафора 1" << '\n';
        exit(EXIT_FAILURE);
    }

    if (sem_init(&shared_memory2->semaphore2, 1, 0) == -1) {
        std::cerr << "Ошибка при инициализации семафора 2" << '\n';
        exit(EXIT_FAILURE);
    }


    pid_t pid1 = fork();
    if (pid1 == -1) {
        std::cout << "Ошибка при создании дочернего процесса" << '\n';
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) {
        int file1 = open(name1, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
        if (dup2(file1, STDOUT_FILENO) == -1) {
            perror("dup2 error ");
            exit(-1);
        }
        temp = execl("./child1", "./child1", NULL);
        if (temp == -1) {
            std::cerr << "Ошибка при запуске программы в дочернем процессе" << '\n';
            exit(EXIT_FAILURE);
        }
    }
    pid_t pid2 = fork();
    if (pid2 == -1) {
        std::cout << "Ошибка при создании дочернего процесса" << '\n';
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {
        int file2 = open(name2, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
        if (dup2(file2, STDOUT_FILENO) == -1) {
            perror("dup2 error ");
            exit(-1);
        }
        temp = execl("./child2", "./child2", NULL);
        if (temp == -1) {
            std::cerr << "Ошибка при запуске программы в дочернем процессе" << '\n';
            exit(EXIT_FAILURE);
        }
    }

    if (pid1 > 0 && pid2 > 0) {
        std::string str;
        std::getline(std::cin, str);
        while (std::getline(std::cin, str)) {
            str += '\n';
            if (str.size() > 11) {
                strncpy(shared_memory1->data, str.c_str(), str.size() + 1);
                //std::cout << shared_memory1->data;
                sem_post(&shared_memory1->semaphore1);
                sem_wait(&shared_memory1->semaphore2);
            } else {
                strncpy(shared_memory2->data, str.c_str(), str.size() + 1);
                sem_post(&shared_memory2->semaphore1);
                sem_wait(&shared_memory2->semaphore2);
            }
            std::cout << "yappi" << std::endl;
        }

        str = '\n';
        strncpy(shared_memory1->data, str.c_str(), str.size());
        strncpy(shared_memory2->data, str.c_str(), str.size());
        sem_post(&shared_memory1->semaphore1);
        sem_wait(&shared_memory1->semaphore2);
        sem_post(&shared_memory2->semaphore1);
        sem_wait(&shared_memory2->semaphore2);

        wait(NULL);
        wait(NULL);

        sem_destroy(&shared_memory1->semaphore1);
        sem_destroy(&shared_memory2->semaphore1);
        sem_destroy(&shared_memory1->semaphore2);
        sem_destroy(&shared_memory2->semaphore2);

        munmap(shared_memory1, sizeof(SharedMemory));
        shm_unlink(shared_memory_name1);
        munmap(shared_memory2, sizeof(SharedMemory));
        shm_unlink(shared_memory_name2);
    }
}