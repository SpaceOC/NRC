#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "Core/base/print.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/data/data_manager.h"

bool dataManager::valueExist(std::filesystem::path filePath, std::string value) {
	fileManager FM;
	nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

	for (auto it = data.begin(); it != data.end(); ++it)
		if (it.value() == value)
			return true;
	return false;
}

bool dataManager::keyExist(std::filesystem::path filePath, std::string key) {
	fileManager FM;
	nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

	for (auto it = data.begin(); it != data.end(); ++it)
		if (it.key() == key)
			return true;
	return false;
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
		std::map<std::string, std::string> temp;

		for (size_t i = 0; i <= (keys.size() - 1); i++)
			temp[keys[i]] = values[i];
		jsonData = temp;
		file << jsonData.dump(4) << '\n';
		file.close();
	}
}

void dataManager::createVectorData(std::filesystem::path filePath, std::vector<std::string> values) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream file(filePath, std::ios::out);
		nlohmann::json jsonData = values;
		file << jsonData.dump(4) << '\n';
		file.close();
	}
}

void dataManager::deleteData(std::filesystem::path filePath, std::string key) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream fileTemp(filePathTemp, std::ios::out);
		std::map<std::string, std::string> notJsonTemp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			notJsonTemp[it.key()] = it.value();
		notJsonTemp.erase(key);
		nlohmann::json newJsonData = notJsonTemp;
		fileTemp << newJsonData.dump(4);
		fileTemp.close();
		remove(filePath);
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

void dataManager::deleteVectorData(std::filesystem::path filePath, std::string value) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream fileTemp(filePathTemp, std::ios::out);
		std::vector<std::string> temp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			if (it.value() != value) 
				temp.push_back(it.value());
		nlohmann::json newJsonData = temp;
		fileTemp << newJsonData.dump(4);
		fileTemp.close();
		remove(filePath);
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

void dataManager::changeData(std::filesystem::path filePath, std::string key, std::string newValue, bool changeKey) {
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

		if (changeKey)
			notJsonTemp[newValue] = temp; // создаём новый ключ со старым значением
		else
			notJsonTemp[temp2] = newValue; // создание того же ключа, но с новым значением
		nlohmann::json newJsonData = notJsonTemp;
		fileTemp << newJsonData.dump(4);
		fileTemp.close();
		remove(filePath);
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

void dataManager::changeVectorData(std::filesystem::path filePath, std::string oldValue, std::string newValue) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream fileTemp(filePathTemp, std::ios::out);
		std::vector<std::string> temp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it) {
			if (it.value() == oldValue) 
				temp.push_back(newValue);
			else
				temp.push_back(it.value());
		}
		nlohmann::json newJsonData = temp;
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

		for (auto it = data.begin(); it != data.end(); ++it)
			notJsonTemp[it.key()] = it.value();
		std::ofstream file(filePathTemp);
		notJsonTemp[key] = value;
		nlohmann::json newJsonData = notJsonTemp;
		file << newJsonData.dump(4);
		file.close();
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

void dataManager::addData(std::filesystem::path filePath, std::vector<std::string> keys, std::vector<std::string> values) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::map<std::string, std::string> notJsonTemp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			notJsonTemp[it.key()] = it.value();
		std::ofstream file(filePathTemp);

		for (size_t i = 0; i <= (keys.size() - 1); i++) 
			notJsonTemp[keys[i]] = values[i];
		nlohmann::json newJsonData = notJsonTemp;
		file << newJsonData.dump(4);
		file.close();
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

void dataManager::addVectorData(std::filesystem::path filePath, std::string value) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::vector<std::string> temp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			temp.push_back(it.value());
		std::ofstream file(filePathTemp);
		temp.push_back(value);
		nlohmann::json newJsonData = temp;
		file << newJsonData.dump(4);
		file.close();
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

void dataManager::addVectorData(std::filesystem::path filePath, std::vector<std::string> values) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		std::vector<std::string> temp;
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			temp.push_back(it.value());
		std::ofstream file(filePathTemp);

		for (auto value : values)
			temp.push_back(value);
		nlohmann::json newJsonData = temp;
		file << newJsonData.dump(4);
		file.close();
		FM.renameFile(filePathTemp, filePath);
		countTemps++;
	}
}

std::string dataManager::getValue(std::filesystem::path filePath, std::string key) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			if (it.key() == key)
				return it.value();
	}
	return "";
}

std::string dataManager::getVectorValue(std::filesystem::path filePath, std::string value) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			if (it.value() == value)
				return it.value();
	}
	return "";
}

std::map<std::string, std::string> dataManager::readAllData(std::filesystem::path filePath) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		std::map<std::string, std::string> temp;

		for (auto it = data.begin(); it != data.end(); ++it)
			temp[it.key()] = it.value();
		return temp;
	}
	return {};
}

std::vector<std::string> dataManager::readAllVectorData(std::filesystem::path filePath) {
	fileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		std::vector<std::string> temp;

		for (auto it = data.begin(); it != data.end(); ++it)
			temp.push_back(it.value());
		return temp;
	}
	return {};
}