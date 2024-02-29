#include "tree.hpp"

int main(int argc, char* argv[]) {
    int id = std::stoi(std::string(argv[1]));
    std::map<std::string, int> stor;

    void* parent_context = zmq_ctx_new();
    void* parent_socket = zmq_socket(parent_context,ZMQ_PAIR);

    int rc = zmq_connect(parent_socket, ("tcp://localhost:" + std::to_string(PORT_BASE + id)).c_str());
    assert (rc == 0);

    std::pair<void*, void*> left, right; // <context, socket>
    std::pair<int, int> child_ids;

    while(1) {
        std::string input;
        recv(parent_socket, input, 0);
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        std::string command = tokens[0];
        if (command == "create") {
            int new_id = std::stoi(tokens[1]);
            create(left, right, child_ids, new_id, id);
        }
        if (command == "exec") {
            int node_id = std::stoi(tokens[1]);
            if (node_id == id) {
                if (tokens.size() == 3) {
                    std::string name = tokens[2];
                    auto it = stor.find(name);
                    if (it == stor.end()) {
                        std::cout << "OK:" << id << " '" << name << "' not found" << std::endl;
                    } else {
                        std::cout << "OK:" << id << " " << it->second << std::endl;
                    }
                }
                if (tokens.size() == 4) {
                    std::string name = tokens[2];
                    int val = std::stoi(tokens[3]);
                    auto it = stor.find(name);
                    if (it == stor.end()) {
                        stor[name] = val;
                        std::cout << "OK:" << id << std::endl;
                    } else {
                        stor[name] = val;
                        std::cout << "OK:" << id << " '" << name << "' is changed to " << val << std::endl;
                    }
                }
            }
            if (node_id < id) {
                send(left.second, input, 1);
            }
            if (node_id > id){
                send(right.second, input, 1);
            }
        }
        if (command == "ping") {
            std::string msg {"good"};
            send(parent_socket, msg, 0);
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
    zmq_close(parent_socket);
    zmq_ctx_destroy(parent_context);
    return 0;
}