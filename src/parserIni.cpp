#include "./parserIni.h"

std::string config = "<config>";
std::string configClose = "</config>";
std::string deps = "<deps>";
std::string depsClose = "</deps>";
std::string sec = "<sec>";
std::string secClose = "</sec>";
std::string name = "<name ";
std::string nameClose = "/>";
std::string command = "<command ";
std::string commandClose = "/>";
std::string dep = "<dep ";
std::string depClose = "/>";
std::string value = "value=\"";
std::string close = "/";

void ParserIni::returnResultParsing(std::string fileName)
{
    std::ifstream file(fileName);

    if (!(file.is_open())) {    
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
    
    // Temporary variables for working with strings
    std::string line;

    // Counters of position 
    bool inConfig = false;
    bool inSec = false;
    bool inDeps = false;
    bool isNameInit = false; // one name in section
    size_t countSectionCommand = 0;

    while (std::getline(file, line, '<')) {
        prepareStr(line); 
        // std::cout << line << std::endl;
        if (line.substr(0, config.size()) == config) {
            if(inDeps || inSec || inConfig) {
                std::perror("Error! Wrong formatting");
                exit(EXIT_FAILURE);
            }
            inConfig = true;
        } else if (line.substr(0, configClose.size()) == configClose) {
            if(inDeps || inSec || !inConfig) {
                std::perror("Error! Wrong formatting");
                exit(EXIT_FAILURE);
            }
            inConfig = false;
        } else if (line.substr(0, deps.size()) == deps) { 
            if(inDeps || !inSec || !inConfig) {
                std::perror("Error! Wrong formatting");
                exit(EXIT_FAILURE);
            }
            inDeps = true;
        } else if (line.substr(0, depsClose.size()) == depsClose) { 
            if(!inDeps || !inSec || !inConfig) {
                std::perror("Error! Wrong formatting");
                exit(EXIT_FAILURE);
            }
            inDeps = false;
        } else if (line.substr(0, sec.size()) == sec) { 
            if(inSec || !inConfig || inDeps) {
                std::perror("Error! Wrong formatting");
                exit(EXIT_FAILURE);
            }
            inSec = true;
        } else if (line.substr(0, secClose.size()) == secClose) { 
            if(!inSec || !inConfig || inDeps) {
                std::perror("Error! Wrong formatting");
                exit(EXIT_FAILURE);
            }
            inSec = false;
            isNameInit = false;
            countSectionCommand++;
        } else if (line.substr(0, name.size())  == name) {
            if(isNameInit) {
                std::perror("Error! More than one <name> in section");
                exit(EXIT_FAILURE);
            }
            if (!(inSec && inConfig) || inDeps) {
                std::perror("Error! Config with Section is not inited or wrong formatting");
                exit(EXIT_FAILURE);
            }
            if (line.substr(line.size() - nameClose.size() - 1, nameClose.size()) != nameClose) {
                std::perror("Error! Name is not closed");
                exit(EXIT_FAILURE);
            }

            std::string extractedName = extractValue(line);

            // std::cout << "name " << extractedName << std::endl;
            _parseRersult.push_back({extractedName, {}, {}});
            isNameInit = true;
        } else if (line.substr(0, command.size())  == command) { 
            if (!(inSec && inConfig) || inDeps) {
                std::perror("Error! Config with Section is not inited or wrong formatting");
                exit(EXIT_FAILURE);
            }
            
            if (line.substr(line.size() - commandClose.size() - 1, commandClose.size()) != commandClose) {
                std::perror("Error! Command is not closed");
                exit(EXIT_FAILURE);
            }

            std::string extractedCommand = extractValue(line);

            // std::cout << "command " << extractedCommand << std::endl;
            _parseRersult[countSectionCommand].command = extractedCommand;
        } else if (line.substr(0, dep.size())  == dep) { 
            if (!(inSec && inConfig && inDeps)) {
                std::perror("Error! Config with Section with Dependencies is not inited");
                exit(EXIT_FAILURE);
            }
            if (line.substr(line.size() - depClose.size() - 1, depClose.size()) != depClose) {
                std::perror("Error! Dependency is not closed");
                exit(EXIT_FAILURE);
            }

            std::string extractedDep = extractValue(line);

            // std::cout << "dep " << extractedDep << std::endl;
            
            _parseRersult[countSectionCommand].dependencies.push_back(extractedDep);

        } else if ((line.size() != 0) && line != "<") {
            std::perror("Error! Unknow command");
            std::cout << "\n\t" << line << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    file.close();
}

std::vector<Job> ParserIni::getParserRes()
{
    return this->_parseRersult;
}

std::string ParserIni::extractValue(std::string& line)
{
    size_t valueStartPos = line.find("value=\"") + value.size();
    size_t valueEndPos = line.find("\"", valueStartPos);
    size_t length = valueEndPos - valueStartPos ;
    if(valueStartPos >= line.size() || valueEndPos >= line.size() || length <= 0) {
        std::perror("Error! Value is not found");
        exit(EXIT_FAILURE);
    }
    std::string extractedValue = line.substr(valueStartPos, length);
    return extractedValue;
}

void ParserIni::prepareStr(std::string& line)
{
    if(line.find(close) < line.size()) {
        line.erase (std::remove(line.begin() + line.find("/"), line.end(), ' '), line.end()); // delete spaces
    }
    line = "<" + line;
}
