#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>

class dataManager {
private:
	static inline int countTemps = 0;
	std::filesystem::path filePathTemp = "Temp/" + std::to_string(countTemps) + "_File.temp";
public:
	bool valueExist(std::filesystem::path filePath, std::string value);

	bool keyExist(std::filesystem::path filePath, std::string key);

	void createData(std::filesystem::path filePath, std::string key, std::string value);
	void createData(std::filesystem::path filePath, std::vector<std::string> keys, std::vector<std::string> values);

	void deleteData(std::filesystem::path filePath, std::string key);

	void changeData(std::filesystem::path filePath, std::string key, std::string new_value, bool changeKey);

	void addData(std::filesystem::path filePath, std::string key, std::string value);
	std::string getValue(std::filesystem::path filePath, std::string key);

	std::string getKey(std::filesystem::path filePath, std::string value);

	std::map<std::string, std::string> readAllData(std::filesystem::path filePath);

};