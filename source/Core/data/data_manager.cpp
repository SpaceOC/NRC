#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include "file_manager.cpp"
#include "../thirdPartyLibraries/include/nlohmann/json.hpp"

class dataManager {
    private:
        static inline int countTemps = 0;
        std::filesystem::path filePathTemp = "Temp/" + std::to_string(countTemps) + "_File.temp";
    public:
        bool valueExist(const std::filesystem::path filePath, std::string value) const {
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

        bool keyExist(const std::filesystem::path filePath, std::string key) const {
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

        void createData(const std::filesystem::path filePath, const std::string key, const std::string value) const {
            fileManager FM;
            if (FM.fileExist(filePath)) {
                std::ofstream file(filePath, std::ios::out);
                nlohmann::json jsonData;
                jsonData[key] = value;
                file << jsonData.dump(4) << '\n';
                file.close();
            }
        }
        void createData(const std::filesystem::path filePath, const std::vector<std::string> keys, const std::vector<std::string> values) const {
            fileManager FM;
            if (FM.fileExist(filePath)) {
                std::ofstream file(filePath, std::ios::out);
                nlohmann::json jsonData;
                std::map<std::string, std::string> tempMap;
                int i = 0;
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
        void deleteData(const std::filesystem::path filePath, const std::string key) const {
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
        void changeData(const std::filesystem::path filePath, const std::string key, const std::string new_value, bool changeKey) const {
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
        void addData(const std::filesystem::path filePath, const std::string key, const std::string value) const {
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
        std::string getValue(const std::filesystem::path filePath, const std::string key) const {
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

        std::string getKey(const std::filesystem::path filePath, const std::string value) const {
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

        std::map<std::string, std::string> readAllData(const std::filesystem::path filePath) const {
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
};