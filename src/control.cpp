#include "tree.hpp"

int main(void) {
    int id = 10;
    std::pair<void*, void*> children;
    std::pair<int, int> child_ids;

    void* context = zmq_ctx_new();
    
    while (1) {
        std::string input;
        std::cout << "Введите команду: ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            std::cout << "Не введены аргументы" << std::endl;
            return 1;
        }
        
        std::string command = tokens[0];
        if (command == "create") {
            int new_id = std::stoi(tokens[1]);
            create(children, child_ids, context, new_id, id);
        }
        if (command == "exec") {
            int node_id = std::stoi(tokens[1]);
            if (node_id < id) {
                send(children.first, input);
            } else {
                send(children.second, input);
            }
        }
        if (command == "pingall") {
            ping(children, child_ids);
            send(children.first, input);
            send(children.second, input);
        }
        if (command == "end") {
            send(children.first, input);
            send(children.second, input);
            break;
        }
    }
    sleep(5);
    zmq_close(children.first);
    zmq_close(children.second);
    zmq_ctx_destroy(context);
    return 0;
}