#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <map> 
#include <sstream>
#include <thread>
#include <chrono>
#include <assert.h>

#define PORT_BASE 5000

void send(void* socket, std::string& message) {
    if (socket != nullptr) {
        zmq_msg_t msg;
        zmq_msg_init(&msg);
        int rc = zmq_msg_init_size(&msg, message.size()); // Инициализация с указанием размера
        assert(rc == 0);
        memcpy(zmq_msg_data(&msg), message.c_str(), message.size());
        rc = zmq_msg_send(&msg, socket, 0); // Отправка сообщения
        assert(rc != -1);
        zmq_msg_close(&msg);
    }
}

bool recv(void *socket, std::string &reply_data, int flag) {
    if (flag == 0) {
        int rc = 0;
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        rc = zmq_msg_recv(&reply, socket, 0);
        if (rc == -1) {
            zmq_msg_close(&reply);
            return false;
        }
        size_t size = zmq_msg_size(&reply);
        std::string str(size, '\0');
        memcpy(&str[0], zmq_msg_data(&reply), size);
        reply_data = str;
        zmq_msg_close(&reply);
        return true;
    } else {
        int rc = 0;
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        rc = zmq_msg_recv(&reply, socket, ZMQ_DONTWAIT);
        if (rc == -1) {
            zmq_msg_close(&reply);
            return false;
        }
        size_t size = zmq_msg_size(&reply);
        std::string str(size, '\0');
        memcpy(&str[0], zmq_msg_data(&reply), size);
        reply_data = str;
        zmq_msg_close(&reply);
        return true;
    }
}

void create(std::pair<void*, void*> children, std::pair<int, int>& child_ids, void* context, int new_id, int cur_id) {
    if (new_id == cur_id) {
        std::cout << "Error:" << new_id << " already exists" << std::endl;
    } else if (new_id < cur_id) {
        if (children.first == nullptr) {
            children.first = zmq_socket(context, ZMQ_PAIR);
            child_ids.first = new_id;
            zmq_connect(children.first, ("tcp://localhost:" + std::to_string(PORT_BASE + new_id)).c_str());
            pid_t pid1 = fork();
            if (pid1 == 0){
                execl("./calc", "./calc", std::to_string(new_id).c_str(), nullptr);
            } 
        } else {
            std::string command {"create " + std::to_string(new_id)};
            send(children.first, command);
        }
    } else {
        if (children.second == nullptr) {
            children.second = zmq_socket(context, ZMQ_PAIR);
            child_ids.second = new_id;
            zmq_connect(children.second, ("tcp://localhost:" + std::to_string(PORT_BASE + new_id)).c_str());
            pid_t pid1 = fork();
            if (pid1 == 0){
                execl("./calc", "./calc", std::to_string(new_id).c_str(), nullptr);
            } 
        } else {
            std::string command {"create " + std::to_string(new_id)};
            send(children.second, command);
        }
    }
}

void ping(std::pair<void*, void*> children, std::pair<int, int>& child_ids) {
    std::string msg {"ping"};
    std::string rep;
    if (children.first != nullptr) {
        send(children.first, msg);
        sleep(3);
        if (recv(children.first, rep, 1)) {
            std::cout << "OK:" << child_ids.first << " is availible" << std::endl; 
        } else {
            std::cout << "OK:" << child_ids.first << " is not availible" << std::endl; 
        }
    }
    if (children.second != nullptr) {
        send(children.second, msg);
        sleep(3);
        if (recv(children.second, rep, 1)) {
            std::cout << "OK:" << child_ids.second << " is availible" << std::endl; 
        } else {
            std::cout << "OK:" << child_ids.second << " is not availible" << std::endl; 
        }
    }
    
}