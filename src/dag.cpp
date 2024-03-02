#include "./dag.h"

void DAG_J::createDAG(std::string fileName)
{
    _jobs.returnResultParsing(fileName);
    std::vector<Job> vecJobs = _jobs.getParserRes();

    for (const auto& job : vecJobs) {
        UJob ujob;
        ujob.j = job;
        _graph.push_back(ujob);
    }   

    for (size_t i = 0; i < _graph.size(); ++i) {
        for (const auto& dep : _graph[i].j.dependencies) {
            // Находим индекс задачи, от которой зависит текущая задача
            auto it = std::find_if(_graph.begin(), _graph.end(), [&dep](const UJob& uj) {
                return uj.j.name == dep;
            });

            if (it != _graph.end()) {
                _graph[i].dep.push_back(std::distance(_graph.begin(), it));
            }
        }
    }    
}

void DAG_J::printDAG()
{
    if (_graph.size() == 0) {
        std::perror("Error! graph is empty");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < _graph.size(); ++i) {
        if (_graph[i].dep.size() == 0) {
            std::cout << _graph[i].j.name << " " << '\n';
        } else {
            std::cout << _graph[i].j.name << " " << " : ";
            for (size_t j = 0; j < _graph[i].dep.size(); ++j) {
                int ind = _graph[i].dep[j];
                std::cout << _graph[ind].j.name << " ";
            }
            std::cout << ";\n";
        }
    }
}

bool DAG_J::hasCycle() {
    std::vector<bool> visited(_graph.size(), false);
    std::vector<bool> recStack(_graph.size(), false);

    for (size_t i = 0; i < _graph.size(); ++i) {
        if (!visited[i] && hasCycleUtil(i, visited, recStack)) {
            std::cout << "В вашем графе найден цикл, пожалуйста, исправьте его!" << std::endl;
            return true; // Найден цикл
        }
    }

    return false; // Цикл не найден
}

bool DAG_J::hasCycleUtil(int v, std::vector<bool>& visited, std::vector<bool>& recStack) {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        for (const auto& adj : _graph[v].dep) {
            if (!visited[adj] && hasCycleUtil(adj, visited, recStack)) {
                return true;
            } else if (recStack[adj]) {
                return true;
            }
        }
    }

    recStack[v] = false; // Убираем вершину из стека рекурсии
    return false;
}

void DAG_J::runCompile() {
    if (hasCycle()) {
        exit(EXIT_FAILURE);
    }

    std::queue<int> readyJobs;
    std::unordered_set<int> completedJobs;

    // Находим все джобы без зависимостей (готовые к выполнению) и добавляем их в очередь
    for (size_t i = 0; i < _graph.size(); ++i) {
        if (_graph[i].j.dependencies.empty()) {
            readyJobs.push(i);
        }
    }

    // Запускаем выполнение джоб в нескольких процессах
    while (!readyJobs.empty()) {
        std::vector<pid_t> childPids;

        // Ограничим максимальное количество процессов для запуска
        int maxProcesses = 5; // Например, 5 процессов
        int jobIndex;
        while (!readyJobs.empty() && maxProcesses > 0) {
            jobIndex = readyJobs.front();
            readyJobs.pop();

            pid_t pid = fork();

            if (pid < 0) {
                std::cout << "Ошибка при создании процесса-потомка." << std::endl;
                exit(EXIT_FAILURE);
            }

            if (pid == 0) {
                // child process
                executeJob(jobIndex);
                exit(EXIT_SUCCESS);
            } else {
                // parent process
                childPids.push_back(pid);
                --maxProcesses;
            }
        }

        // Ожидаем завершение выполнения дочерних процессов
        for (pid_t pid : childPids) {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exitStatus = WEXITSTATUS(status);
                if (exitStatus == EXIT_SUCCESS) {
                    completedJobs.insert(jobIndex);
                    maxProcesses++;
                }
            }
        }

        // Находим новые готовые джобы для выполнения   
        auto isJobCompleted = [&](const std::string& dep) {
            auto it = std::find_if(_graph.begin(), _graph.end(),
                                [&dep](const UJob& uj) {
                                    return uj.j.name == dep;
                                });
            return it != _graph.end() && completedJobs.find(std::distance(_graph.begin(), it)) != completedJobs.end();
        };

        for (size_t i = 0; i < _graph.size(); ++i) {
            if (completedJobs.find(i) == completedJobs.end() &&
                std::all_of(_graph[i].j.dependencies.begin(), _graph[i].j.dependencies.end(), isJobCompleted)) {
                readyJobs.push(i);
            }
        }
    }
}

void DAG_J::executeJob(int jobIndex) {
    std::string compileCommand = _graph[jobIndex].j.command;

    if (!compileCommand.empty()) {
        std::cout << "Выполнение команды: " << compileCommand << std::endl;

        // Выполняем команду в процессе-потомке
        FILE* pipe = popen(compileCommand.c_str(), "r");
        if (pipe) {
            pclose(pipe);
        } else {
            std::cout << "Ошибка при выполнении команды компиляции." << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        std::cout << "Команда компиляции для джобы не найдена." << std::endl;
        exit(EXIT_FAILURE);
    }
}