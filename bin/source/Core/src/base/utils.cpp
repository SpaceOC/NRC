#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Core/base/utils.h"

core::Utils::Utils() {}

bool core::Utils::stringIsNumbers(const std::string& content) {
    for (const auto& letter : content) {

        if (letter == '-' && letter == content.at(0))
            continue;  
        else if (!isdigit(letter))
            return false;
    }
    return true;
}

std::vector<std::string> core::Utils::split(const std::string& content, const char what) {
    if (content.empty())
        return {};

    std::vector<std::string> result;
    std::string temp;
    std::istringstream tokenStream(content);
    while (std::getline(tokenStream, temp, what))
        result.push_back(temp);
    return result;
}

std::string core::Utils::replace(std::string& content, const std::string& a, const std::string& b) {
    size_t i = content.find(a);
    while (i != std::string::npos) {
        content.erase(i, 3);
        content.insert(i, b);
        i = content.find(a);
    }
    return content;
}

std::string core::Utils::getFileContent(const std::string& path) {
	std::ifstream file;
    file.open(path, std::ios::in);
    if (!file.is_open()) return "";
	std::string line, temp;
	while (std::getline(file, line)) { temp += line; }
	file.close();
	return temp;
}

bool core::Utils::endsWith(const std::string& str, const std::string& str2) {
    return str.substr(str.length() - str2.length(), str.length()) == str2;
}