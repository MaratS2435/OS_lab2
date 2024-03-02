#pragma once
#include "node.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <map>

std::string openXml(const std::string& xmlFilePath);

void parseGraph(const std::string& xmlContent, std::map<int, Node>& graph);

void parseNodes(const std::string& xmlContent, std::vector<int>& startNodes, std::vector<int>& endNodes);