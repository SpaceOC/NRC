/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

bool core::DataManager::valueExist(std::filesystem::path filePath, std::string value) {
	FileManager FM;
	nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

	for (auto it = data.begin(); it != data.end(); ++it)
		if (it.value() == value)
			return true;
	return false;
}

bool core::DataManager::keyExist(std::filesystem::path filePath, std::string key) {
	FileManager FM;
	nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

	for (auto it = data.begin(); it != data.end(); ++it)
		if (it.key() == key)
			return true;
	return false;
}

void core::DataManager::createData(std::filesystem::path filePath, std::string key, std::string value) {
	FileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream file(filePath, std::ios::out);
		nlohmann::json jsonData;
		jsonData[key] = value;
		file << jsonData.dump(4) << '\n';
		file.close();
	}
}

void core::DataManager::createData(std::filesystem::path filePath, std::vector<std::string> keys, std::vector<std::string> values) {
	FileManager FM;
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

void core::DataManager::createVectorData(std::filesystem::path filePath, std::vector<std::string> values) {
	FileManager FM;
	if (FM.fileExist(filePath)) {
		std::ofstream file(filePath, std::ios::out);
		nlohmann::json jsonData = values;
		file << jsonData.dump(4) << '\n';
		file.close();
	}
}

void core::DataManager::deleteData(std::filesystem::path filePath, std::string key) {
	FileManager FM;
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

void core::DataManager::deleteVectorData(std::filesystem::path filePath, std::string value) {
	FileManager FM;
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

void core::DataManager::changeData(std::filesystem::path filePath, std::string key, std::string newValue, bool changeKey) {
	FileManager FM;
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

void core::DataManager::changeVectorData(std::filesystem::path filePath, std::string oldValue, std::string newValue) {
	FileManager FM;
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

void core::DataManager::addData(std::filesystem::path filePath, std::string key, std::string value) {
	FileManager FM;
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

void core::DataManager::addData(std::filesystem::path filePath, std::vector<std::string> keys, std::vector<std::string> values) {
	FileManager FM;
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

void core::DataManager::addVectorData(std::filesystem::path filePath, std::string value) {
	FileManager FM;
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

void core::DataManager::addVectorData(std::filesystem::path filePath, std::vector<std::string> values) {
	FileManager FM;
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

std::string core::DataManager::getValue(std::filesystem::path filePath, std::string key) {
	FileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			if (it.key() == key)
				return it.value();
	}
	return "";
}

std::string core::DataManager::getVectorValue(std::filesystem::path filePath, std::string value) {
	FileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));

		for (auto it = data.begin(); it != data.end(); ++it)
			if (it.value() == value)
				return it.value();
	}
	return "";
}

std::map<std::string, std::string> core::DataManager::readAllData(std::filesystem::path filePath) {
	FileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		std::map<std::string, std::string> temp;

		for (auto it = data.begin(); it != data.end(); ++it)
			temp[it.key()] = it.value();
		return temp;
	}
	return {};
}

std::vector<std::string> core::DataManager::readAllVectorData(std::filesystem::path filePath) {
	FileManager FM;
	if (FM.fileExist(filePath)) {
		nlohmann::json data = nlohmann::json::parse(FM.readFile(filePath));
		std::vector<std::string> temp;

		for (auto it = data.begin(); it != data.end(); ++it)
			temp.push_back(it.value());
		return temp;
	}
	return {};
}