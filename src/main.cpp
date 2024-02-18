#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
/*
size_t change(char **output, char* input) {
    size_t size = strlen(input) + 1;
    char* new_s = (char*)malloc(sizeof(char) * strlen(input) + 1);
    size_t i = 0, j = 0;

    for (i = 0; i < strlen(input); ++i) {
        if (input[i] != 'A' && input[i] != 'E' && input[i] != 'I' && input[i] != 'O' && input[i] != 'Y' && input[i] != 'U'
         && input[i] != 'a' && input[i] != 'e' && input[i] != 'i' && input[i] != 'o' && input[i] != 'y' && input[i] != 'u') {
            
            new_s[i - j] = input[i];

        } else {
            size--;
            j++;
        }
    }
    new_s[i - j] = '\n';

    *output = new_s;
    return size;
}*/

std::string change(std::string& s) {
    std::string new_s;
    size_t i = 0, j = 0;

    for (i = 0; i < s.size(); ++i) {
        if (s[i] != 'A' && s[i] != 'E' && s[i] != 'I' && s[i] != 'O' && s[i] != 'Y' && s[i] != 'U'
         && s[i] != 'a' && s[i] != 'e' && s[i] != 'i' && s[i] != 'o' && s[i] != 'y' && s[i] != 'u') {
            
            new_s[i - j] = s[i];

        } else {
            j++;
        }
    }
    new_s[i - j] = '\n';

    return new_s;
}

int main() {
    int pageSize = getpagesize();
    std::string fileName1, fileName2;
    std::cin >> fileName1 >> fileName2;
    const char * name1 = fileName1.c_str();
    const char * name2 = fileName2.c_str();
    int file1 = open(name1, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
    struct stat size1;
    fstat(file1, &size1);
    if (size1.st_size == 0) {
        ftruncate(file1, 1);
        fstat(file1, &size1);
    }
    int file2 = open(name2, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
    struct stat size2;
    fstat(file2, &size2);
    if (size2.st_size == 0) {
        ftruncate(file2, 1);
        fstat(file2, &size2);
    }
    char * mFile1 = (char *)mmap(NULL, size1.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file1, 0);
    char * mFile2 = (char *)mmap(NULL, size2.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file2, 0);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork1");
        return -1;
    }
    pid_t pid2;
    if (pid > 0) {
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork2");
            return -1;
        } else if (pid2 == 0) {
            pid = 1;
        }
    }
    if (pid == 0) {
        pid2 = 1;
    }

    if (pid > 0 && pid2 > 0) {
        std::string s, s1, s2;
        int flag = 0;
        while (std::cin >> s) {
            if (s.size() <= 10) {
                s1 += s;
                s1 += "\n";
            } else {
                s2 += s;
                s2 += "\n";
            }

            /*if (flag == 0) {
                s1 += s;
                s1 += "\n";
                flag = 1;
            } else {
                s2 += s;
                s2 += "\n";
                flag = 0;
            }*/
        }
        mFile1 = (char *)mremap(mFile1, size1.st_size, s1.size(), MREMAP_MAYMOVE);
        if (mFile1 == (char *)-1) {
            perror("mmap1");
            return -1;
        }
        size1.st_size = s1.size();
        memcpy(mFile1, s1.c_str(), s1.size());
        mFile2 = (char *)mremap(mFile2, size2.st_size, s2.size(), MREMAP_MAYMOVE);
        if (mFile1 == (char *)-1) {
            perror("mmap2");
            return -1;
        }
        size2.st_size = s2.size();
        strcpy((char *)mFile2, s2.c_str());
        ftruncate(file1, s1.size());
        ftruncate(file2, s2.size());
        if (munmap(mFile1, s1.size()) < 0) {
            perror("close mmf1");
        }
        if (munmap(mFile2, s2.size()) < 0) {
            perror("close mmf2");
        }
    } if (pid == 0) {
        std::string s = mFile1;
        std::string s1 = change(s);
        memcpy(mFile1, s1.c_str(), s1.size());
        if (munmap(mFile1, s1.size()) < 0) {
            perror("close mmf1");
        }
    } else if (pid2 == 0) {
        std::string s = mFile2;
        std::string s1 = change(s);
        memcpy(mFile2, s1.c_str(), s1.size());
        if (munmap(mFile2, s1.size()) < 0) {
            perror("close mmf2");
        }
    }
    close(file1);
    close(file2);
    return 0;
}
