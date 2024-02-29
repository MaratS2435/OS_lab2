#include "tree.hpp"

int main(void) {
    int id = 10;
    std::pair<void*, void*> left, right; // <context, socket>
    std::pair<int, int> child_ids;

    
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
            create(left, right, child_ids, new_id, id);
        }
        if (command == "exec") {
            int node_id = std::stoi(tokens[1]);
            if (node_id < id) {
                send(left.second, input, 1);
            } else if (node_id > id) {
                send(right.second, input, 1);
            } else {
                std::cout << "Error: control node isn't a calculation node" << std::endl;
            }
        }
        if (command == "pingall") {
            ping(left, right,  child_ids);
            if (left.second != nullptr) {
                send(left.second, input, 0);
            }
            if (right.second != nullptr) {
                send(right.second, input, 0);
            }
        }
        if (command == "end") {
            if (left.second != nullptr) {
                send(left.second, input, 1);
            }
            if (right.second != nullptr) {
                send(right.second, input, 1);
            }
            break;
        }
    }

    sleep(5);
    if (left.second != nullptr) {
        zmq_close(left.second);
        zmq_ctx_destroy(left.first);
    }
    if (right.second != nullptr) {
        zmq_close(right.second);
        zmq_ctx_destroy(left.first);
    }
    
    return 0;
}