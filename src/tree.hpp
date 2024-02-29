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

void send(void *&socket, std::string& message, int flag) {
    if (flag == 0) {
        if (socket != nullptr) {
            zmq_msg_t msg;
            zmq_msg_init(&msg);
            int rc = zmq_msg_init_size(&msg, message.size()); // Инициализация с указанием размера
            assert(rc == 0);
            memcpy(zmq_msg_data(&msg), message.c_str(), message.size());
            rc = zmq_msg_send(&msg, socket, 0); // Отправка сообщения
            assert(rc != -1);
            zmq_msg_close(&msg);
        } else {
            std::cout << "Error:id doesn't exist" << std::endl;
        }
    } else {
        if (socket != nullptr) {
            zmq_msg_t msg;
            zmq_msg_init(&msg);
            int rc = zmq_msg_init_size(&msg, message.size()); // Инициализация с указанием размера
            assert(rc == 0);
            memcpy(zmq_msg_data(&msg), message.c_str(), message.size());
            rc = zmq_msg_send(&msg, socket, ZMQ_DONTWAIT); // Отправка сообщения
            zmq_msg_close(&msg);
        } else {
            std::cout << "Error:id doesn't exist" << std::endl;
        }
    }
}

bool recv(void *&socket, std::string &reply_data, int flag) {
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

void create(std::pair<void*, void*>& left, std::pair<void*, void*>& right, std::pair<int, int>& child_ids, int new_id, int cur_id) {
    if (new_id == cur_id) {
        std::cout << "Error:" << new_id << " already exists" << std::endl;
    } else if (new_id < cur_id) {
        if (left.second == nullptr) {
            left.first = zmq_ctx_new();
            left.second = zmq_socket(left.first, ZMQ_PAIR);
            child_ids.first = new_id;
            zmq_bind(left.second, ("tcp://*:" + std::to_string(PORT_BASE + new_id)).c_str());
            pid_t pid1 = fork();
            if (pid1 == 0){
                execl("./calc", "./calc", std::to_string(new_id).c_str(), nullptr);
            } else {
                std::cout << "OK:" << pid1 << std::endl;
            }
        } else {
            std::string command {"create " + std::to_string(new_id)};
            send(left.second, command, 1);
        }
    } else {
        if (right.second == nullptr) {
            right.first = zmq_ctx_new();
            right.second = zmq_socket(right.first, ZMQ_PAIR);
            child_ids.second = new_id;
            zmq_bind(right.second, ("tcp://*:" + std::to_string(PORT_BASE + new_id)).c_str());
            pid_t pid1 = fork();
            if (pid1 == 0){
                execl("./calc", "./calc", std::to_string(new_id).c_str(), nullptr);
            } else {
                std::cout << "OK:" << pid1 << std::endl;
            }
        } else {
            std::string command {"create " + std::to_string(new_id)};
            send(right.second, command, 1);
        }
    }
}

void ping(std::pair<void*, void*>& left, std::pair<void*, void*>& right, std::pair<int, int>& child_ids) {
    std::string msg {"ping"};
    std::string rep;
    if (left.second != nullptr) {
        send(left.second, msg, 1);
        sleep(3);
        if (recv(left.second, rep, 1)) {
            std::cout << "OK:" << child_ids.first << " is availible" << std::endl; 
        } else {
            zmq_close(left.second);
            zmq_ctx_destroy(left.first);
            left.first = nullptr;
            left.second = nullptr;
            std::cout << "OK:" << child_ids.first << " is not availible and has been deleted" << std::endl; 
        }
    }
    if (right.second != nullptr) {
        send(right.second, msg, 1);
        sleep(3);
        if (recv(right.second, rep, 1)) {
            std::cout << "OK:" << child_ids.second << " is availible" << std::endl; 
        } else {
            zmq_close(right.second);
            zmq_ctx_destroy(right.first);
            right.first = nullptr;
            right.second = nullptr;
            std::cout << "OK:" << child_ids.second << " is not availible and has been deleted" << std::endl; 
        }
    }
    
}