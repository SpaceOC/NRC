#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include "Core/data/file_manager.h"
#include "Core/data/data_manager.h"
#include "thirdPartyLibraries/include/nlohmann/json.hpp"

bool dataManager::valueExist(std::filesystem::path filePath, std::string value) {
	fileManager FM;
	bool Temp = false;
	nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it.value() == value) {
			Temp = true;
		}
	}
	return Temp;
}

bool dataManager::keyExist(std::filesystem::path filePath, std::string key) {
	fileManager FM;
	bool Temp = false;
	nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it.key() == key) {
			Temp = true;
		}
	}
	return Temp;
}

void dataManager::createData(std::filesystem::path filePath, std::string key, std::string value) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream file(filePath, std::ios::out);
		nlohmann::json jsonData;
		jsonData[key] = value;
		file << jsonData.dump(4) << '\n';
		file.close();
	}
}
void dataManager::createData(std::filesystem::path filePath, std::vector<std::string> keys, std::vector<std::string> values) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream file(filePath, std::ios::out);
		nlohmann::json jsonData;
		std::map<std::string, std::string> tempMap;
		size_t i = 0;
		while (i <= (values.size() - 1)) {
			for (auto key : keys) {
				tempMap[key] = values[i];
				i++;
			}
		}
		jsonData = tempMap;
		file << jsonData.dump(4) << '\n';
		file.close();
	}
}
void dataManager::deleteData(std::filesystem::path filePath, std::string key) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream fileTemp(filePathTemp, std::ios::out);
		std::string temp, temp2;
		std::map<std::string, std::string> notJsonTemp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		for (auto it = data.begin(); it != data.end(); ++it) {
			notJsonTemp[it.key()] = it.value();
		}
		notJsonTemp.erase(key);
		nlohmann::json newJsonData = notJsonTemp;
		fileTemp << newJsonData.dump(4);
		fileTemp.close();
		remove(filePath);
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}
void dataManager::changeData(std::filesystem::path filePath, std::string key, std::string new_value, bool changeKey) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream fileTemp(filePathTemp, std::ios::out);
		std::string temp, temp2;
		std::map<std::string, std::string> notJsonTemp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		for (auto it = data.begin(); it != data.end(); ++it) {
			if (it.key() == key) {
				temp = it.value();
				temp2 = it.key();
			}
			notJsonTemp[it.key()] = it.value();
		}
		notJsonTemp.erase(temp2);
		if (changeKey) {
			notJsonTemp[new_value] = temp; // создаём новый ключ со старым значением
		}
		else {
			notJsonTemp[temp2] = new_value; // создание того же ключа, но с новым значением
		}
		nlohmann::json newJsonData = notJsonTemp;
		fileTemp << newJsonData.dump(4);
		fileTemp.close();
		remove(filePath);
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}
void dataManager::addData(std::filesystem::path filePath, std::string key, std::string value) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::map<std::string, std::string> notJsonTemp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		for (auto it = data.begin(); it != data.end(); ++it) {
			notJsonTemp[it.key()] = it.value();
		}
		std::ofstream file(filePathTemp);
		notJsonTemp[key] = value;
		nlohmann::json newJsonData = notJsonTemp;
		file << newJsonData.dump(4);
		file.close();
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}
std::string dataManager::getValue(std::filesystem::path filePath, std::string key) {
	fileManager FM;
	std::string Temp = "";
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		for (auto it = data.begin(); it != data.end(); ++it) {
			if (it.key() == key) {
				std::cout << it.value() << std::endl;
				Temp = it.value();
			}
		}
	}
	return Temp;
}

std::string dataManager::getKey(std::filesystem::path filePath, std::string value) {
	fileManager FM;
	std::string Temp = "";
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		for (auto it = data.begin(); it != data.end(); ++it) {
			if (it.value() == value) {
				std::cout << it.key() << std::endl;
				Temp = it.key();
			}
		}
	}
	return Temp;
}

std::map<std::string, std::string> dataManager::readAllData(std::filesystem::path filePath) {
	fileManager FM;
	std::map<std::string, std::string> Temp;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		for (auto it = data.begin(); it != data.end(); ++it) {
			Temp[it.key()] = it.value();
		}
	}
	return Temp;
}
