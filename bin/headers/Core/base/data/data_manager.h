// Data Manager header
// by SpaceOC
#pragma once
#ifndef NRC_BASE_DATA_DATA_MANAGER_H_
#define NRC_BASE_DATA_DATA_MANAGER_H_

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
		void createVectorData(std::filesystem::path filePath, std::vector<std::string> values);
		
		void deleteData(std::filesystem::path filePath, std::string key);
		void deleteVectorData(std::filesystem::path filePath, std::string value);
		
		void changeData(std::filesystem::path filePath, std::string key, std::string newValue, bool changeKey = false);
		void changeVectorData(std::filesystem::path filePath, std::string oldValue, std::string newValue);
		
		void addData(std::filesystem::path filePath, std::string key, std::string value);
		void addData(std::filesystem::path filePath, std::vector<std::string> keys, std::vector<std::string> values);
		void addVectorData(std::filesystem::path filePath, std::string value);
		void addVectorData(std::filesystem::path filePath, std::vector<std::string> value);

		std::string getValue(std::filesystem::path filePath, std::string key);
		std::string getVectorValue(std::filesystem::path filePath, std::string value);

		std::map<std::string, std::string> readAllData(std::filesystem::path filePath);
		std::vector<std::string> readAllVectorData(std::filesystem::path filePath);
};
#endif