#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>

typedef struct _job {
    std::string name;
    std::string command;
    std::vector<std::string> dependencies;
} Job;

class ParserIni 
{
public:
    ParserIni() = default;

    ~ParserIni() = default;

    void returnResultParsing(std::string fileName);
    std::vector<Job> getParserRes();

private:
    std::string extractValue(std::string& line);
    void prepareStr(std::string& line);

private:
    std::vector<Job> _parseRersult;
};