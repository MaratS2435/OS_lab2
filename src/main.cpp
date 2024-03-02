#include "node.h"
#include "parser.h"
#include "jobs.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>


void jobsAdd(std::map<std::string, jobFunc>& jobs, const std::map<int, Node>& graph) {
    for (const auto& [_,node] : graph) {
        // Ищите функцию в functionMap по имени и добавляйте ее в jobs map
        if (functionMap.find(node.job) != functionMap.end()) {
            jobs[node.job] = functionMap.at(node.job);
        }
    }
}

bool hasCyclesDFS(const std::map<int, Node>& graph, int node_id, std::set<int>& visited, std::set<int>& curPath) {
    visited.insert(node_id);
    curPath.insert(node_id);

    const Node& current_node = graph.at(node_id);
    for (int connection : current_node.connections) {
        if (visited.find(connection) == visited.end()) {
            if (hasCyclesDFS(graph, connection, visited, curPath)) {
                return true;
            }
        } else if (curPath.find(connection) != curPath.end()) {
            return true;
        }
    }

    curPath.erase(node_id);
    return false;
}

bool hasCycles(const std::map<int, Node>& graph) {
    std::set<int> visited;
    std::set<int> curPath;

    for (const auto& [node_id, _] : graph) {
        if (hasCyclesDFS(graph, node_id, visited, curPath)) {
            return true;
        }
    }

    return false;
}


bool hasStartAndEndNodes(const std::vector<int>& startNodes, const std::vector<int>& endNodes) {
    return !startNodes.empty() && !endNodes.empty();
}

/*void printVisited(const std::set<int>& visited) {
    std::cout << "Visited nodes: ";
    for (int node_id : visited) {
        std::cout << node_id << " ";
    }
    std::cout << std::endl;
}*/

size_t isWeaklyConnected(const std::map<int, Node>& graph) {
    if (graph.empty()) {
        return true; 
    }

    std::set<int> visited;
    std::stack<int> stack;
    stack.push(graph.begin()->first); 

    while (!stack.empty()) {
        int node_id = stack.top();
        stack.pop();

        if (visited.find(node_id) == visited.end()) {
            visited.insert(node_id);

            const Node& current_node = graph.at(node_id);
            for (int connection : current_node.connections) {
                stack.push(connection);
            }
        }
    }
    
    return visited.size(); 
}

bool isGraphConnected(const std::map<int, Node>& graph) {
    std::map<int, Node> undirected_graph;
    for (const auto& [node_id, node] : graph) {
        for (int connection : node.connections) {
            undirected_graph[connection].connections.push_back(node_id);
            undirected_graph[node_id].connections.push_back(connection);
        }
    }
    return isWeaklyConnected(undirected_graph) == graph.size();
}

void jobsRun(const std::map<int, Node>& graph, const std::vector<int>& startNodes, const std::map <std::string, jobFunc>& jobs) {
    std::set<int> visited;
    std::queue<int> queue;
    
    for (int start_node : startNodes) {
        queue.push(start_node);
        visited.insert(start_node);
    }
    
    while (!queue.empty()) {
        int node_id = queue.front();
        queue.pop();
        
        const Node& current_node = graph.at(node_id);
        std::string job_name = current_node.job;
        
        try {
            jobs.at(job_name)();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return;
        }
        
        for (int connection : current_node.connections) {
            if (visited.find(connection) == visited.end()) {
                queue.push(connection);
                visited.insert(connection);
            }
        }
    }
}

int main() {
    std::map<std::string, jobFunc> jobs;
    std::map<int, Node> graph;
    std::vector<int> startNodes;
    std::vector<int> endNodes;

    std::string dagXml = openXml("../src/dags/dag.xml");
    parseGraph(dagXml, graph);
    parseNodes(dagXml, startNodes, endNodes);

    bool has_cycles = hasCycles(graph);
    bool has_connection = isGraphConnected(graph);
    bool has_start_and_end_nodes = hasStartAndEndNodes(startNodes, endNodes);

    if (has_cycles) {
        std::cout << "Has cycles: Yes" << std::endl;
        return 1;
    }
    else{
        std::cout << "Has cycles: No" << std::endl;
    }
    if (!has_start_and_end_nodes) {
        std::cout << "Has start and end nodes: No" << std::endl;
        return 1;
    }
    else{
        std::cout << "Has start and end nodes: Yes" << std::endl;
    }
    if (!has_connection) {
        std::cout << "Has only 1 connection: No" << std::endl;
        return 1;
    }
    else{
        std::cout << "Has only 1 connection: Yes" << std::endl;
    }

    jobsAdd(jobs, graph);
    jobsRun(graph, startNodes, jobs);


        // for (const auto& [node_id, node_data] : graph) {
    //     std::cout << "Node ID: " << node_id << ", Job: " << node_data.job << ", Connections: ";
    //     for (int connection : node_data.connections) {
    //         std::cout << connection << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // for (int start_node : startNodes) {
    //     std::cout << "Start node: " << start_node << std::endl;
    // }
    // for (int end_node : endNodes) {
    //     std::cout << "End node: " << end_node << std::endl;
    // }

    return 0;
}