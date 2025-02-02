#ifndef NRC_BASE_COMMAND_COMAND_PARSER_H_
#define NRC_BASE_COMMAND_COMAND_PARSER_H_

#include <utility>
#include <vector>
#include <string>

namespace core {
    std::string getStringInQ(const std::vector<std::string>& splittedText, int& count);
    std::pair<std::string, std::string> getMapArg(const std::string& targetText);
}

#endif