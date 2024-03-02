
#include "parser.h"

std::string openXml(const std::string& xmlFilePath){
    std::ifstream file(xmlFilePath);
    if (!file.is_open()) {
        std::cout << "Failed to open XML file." << std::endl;
        exit(-1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string xmlContent = buffer.str();
    return xmlContent;
}

void parseGraph(const std::string& xmlContent, std::map<int, Node>& graph) {
    if(xmlContent == "") return;

    size_t pos = 0;
    while ((pos = xmlContent.find("<node", pos)) != std::string::npos) {
        size_t idStart = xmlContent.find("id=", pos) + strlen("id=\"");
        size_t idEnd = xmlContent.find("\"", idStart);
        int nodeId = std::stoi(xmlContent.substr(idStart, idEnd - idStart));

        size_t jobStart = xmlContent.find(">", idEnd) + 1;
        size_t jobEnd = xmlContent.find("<", jobStart);
        std::string job = xmlContent.substr(jobStart, jobEnd - jobStart);

        graph[nodeId] = {job, std::vector<int>()};

        pos = jobEnd;
    }

    pos = 0;
    while ((pos = xmlContent.find("<connection", pos)) != std::string::npos) {
        size_t fromStart = xmlContent.find("from=", pos) + strlen("from=\"");
        size_t fromEnd = xmlContent.find("\"", fromStart);
        int fromNodeId = std::stoi(xmlContent.substr(fromStart, fromEnd - fromStart));

        size_t toStart = xmlContent.find("to=", pos) + strlen("to=\"");
        size_t toEnd = xmlContent.find("\"", toStart);
        int toNodeId = std::stoi(xmlContent.substr(toStart, toEnd - toStart));

        graph[fromNodeId].connections.push_back(toNodeId);

        pos = toEnd;
    }
}

void parseNodes(const std::string& xmlContent, std::vector<int>& startNodes, std::vector<int>& endNodes) {
    if(xmlContent == "") return;

    size_t pos = 0;
    while((pos = xmlContent.find("<start_node>", pos)) != std::string::npos) {
        size_t idStart = xmlContent.find(">", pos) + 1;
        size_t idEnd = xmlContent.find("<", idStart);
        int nodeId = std::stoi(xmlContent.substr(idStart, idEnd - idStart));

        startNodes.push_back(nodeId);

        pos = idEnd;
    }

    pos = 0;
    while((pos = xmlContent.find("<end_node>", pos)) != std::string::npos) {
        size_t idStart = xmlContent.find(">", pos) + 1;
        size_t idEnd = xmlContent.find("<", idStart);
        int nodeId = std::stoi(xmlContent.substr(idStart, idEnd - idStart));

        endNodes.push_back(nodeId);

        pos = idEnd;
    }
}