#pragma once

#include <semaphore.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_SIZE 1024

const char* shared_memory_name1 = "mySharedMemory1";
const char* shared_memory_name2 = "mySharedMemory2";
const int prots = PROT_READ | PROT_WRITE;
const int flags = MAP_SHARED;

struct SharedMemory {
    char data[MAX_SIZE];
    sem_t semaphore1;
    sem_t semaphore2;
};