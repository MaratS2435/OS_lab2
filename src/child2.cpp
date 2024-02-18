#include "shared_memory.hpp"

int main() {
    std::string str;

    int shared_memory_fd = shm_open(shared_memory_name2, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (shared_memory_fd == -1) {
        std::cerr << "Ошибка при открытии shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    SharedMemory* shared_memory = (SharedMemory*)mmap(NULL, sizeof(SharedMemory), prots, flags, shared_memory_fd, 0);
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Ошибка при отображении shared_memory" << '\n';
        exit(EXIT_FAILURE);
    }

    while (1) {
        sem_wait(&shared_memory->semaphore1);
        str = std::string(shared_memory->data);
        if (str == "\n") {
            break;
        }
        write(STDOUT_FILENO, str.c_str(), str.size());
        memset(shared_memory->data, 0, sizeof(shared_memory->data));
        sem_post(&shared_memory->semaphore2);
    }

    sem_post(&shared_memory->semaphore2);

    return 0;
}