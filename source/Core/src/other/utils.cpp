#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Core/other/utils.h"

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

int core::Utils::getIFromStrVersionId(const std::string& str) {
	int result = 0;

/*
	6 / 10 = 0.6
	1 / 100 = 0.01
	0.6 + 0.01 = 0.61

	7 / 10 = 0.7

	0.81 * 100 = 61
	0.7 * 100 = 70
*/

	int rd = 0; // 0.6.1 != 0.7 !!!!
	for (size_t i = 0; i < str.size(); i++) {
		if (!isdigit(str[i])) continue;

		const char* a = str.substr(i, i + 1).c_str();
		int r = atoi(a);
		if (r != 0) result += r / (rd);
		rd += 10 * (!rd ? 1 : rd);
	}

	return result * rd;
}