#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Core/utils/other_util.h"

bool core::other_util::stringIsNumbers(const std::string& content) {
	for (const auto& letter : content) {

		if (letter == '-' && letter == content.at(0))
			continue;  
		else if (!isdigit(letter))
			return false;
	}
	return true;
}

std::string core::other_util::getFileContent(const std::string& path) {
	std::ifstream file;
	file.open(path, std::ios::in);
	if (!file.is_open()) return "";
	std::string line, temp;
	while (std::getline(file, line)) { temp += line; }
	file.close();
	return temp;
}