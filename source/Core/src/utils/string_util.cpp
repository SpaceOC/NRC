#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Core/utils/string_util.h"

std::vector<std::string> core::string_util::split(const std::string& content, const char what) {
	if (content.empty())
		return {};

	std::vector<std::string> result;
	std::string temp;
	std::istringstream tokenStream(content);
	while (std::getline(tokenStream, temp, what))
		result.push_back(temp);
	return result;
}

std::string core::string_util::replace(std::string& content, const std::string& a, const std::string& b) {
	size_t i = content.find(a);
	while (i != std::string::npos) {
		content.erase(i, 3);
		content.insert(i, b);
		i = content.find(a);
	}
	return content;
}

bool core::string_util::endsWith(const std::string& str, const std::string& str2) {
	return str.substr(str.length() - str2.length(), str.length()) == str2;
}