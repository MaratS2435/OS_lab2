#include "tree.hpp"

int main(int argc, char* argv[]) {
    int id = std::stoi(std::string(argv[1]));
    std::map<std::string, int> stor;

    void* context = zmq_ctx_new();
    std::pair<void*, void*> children;
    std::pair<int, int> child_ids;
    void* parent_socket = zmq_socket(context,ZMQ_PAIR);

    int rc = zmq_bind(parent_socket, ("tcp://localhost:" + std::to_string(PORT_BASE + id)).c_str());
    assert (rc == 0);

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
            create(children, child_ids, context, new_id, id);
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
                        std::cout << "OK:" << id << " '" << name << "' is changed to " << val << std::endl;
                    }
                }
                
                break;
            }
            if (node_id < id) {
                send(children.first, input);
            } else {
                send(children.second, input);
            }
        }
        if (command == "ping") {
            std::string msg {"good"};
            send(parent_socket, msg);
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
    zmq_close(parent_socket);
    zmq_ctx_destroy(context);
    return 0;
}