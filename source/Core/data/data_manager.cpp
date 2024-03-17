#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>

class Data_Manager {
    private:

    public:
        virtual std::string getVar(std::filesystem::path FilePath, std::string SelectedVar) const {
            std::ifstream data(FilePath);
            if (data) {
                std::string line;
                bool foundVar = false;
                while (getline(data, line)) {
                    if (!foundVar && line.find(SelectedVar + ": ") != std::string::npos) {
                        return line.substr(SelectedVar.size() + 2);
                        foundVar = true;
                        break;
                    }
                }
                data.close();
            }
            else {
            }
        }
        /*
        virtual void CreateBase(std::filesystem::path FilePath) const {

        }
        */
        virtual void setNewVar(std::filesystem::path FilePath, std::filesystem::path TempFilePath, std::string field, std::string NewValue) const {
            std::ifstream inputFile(FilePath);
            std::ofstream tempFile(TempFilePath);
            if (inputFile && tempFile) {
                std::string line;
            bool fieldFound = false;
            while (getline(inputFile, line)) {
                if (line.find(field)!= std::string::npos) {
                    tempFile << field << ": " << NewValue << std::endl;
                    fieldFound = true;
                }
                else {
                    tempFile << line << '\n';
                }
            }
            inputFile.close();
            tempFile.close();
            remove(FilePath);
            rename(TempFilePath, FilePath);
            if (!fieldFound) {
            }
        }
        else {

            }
        }
        /*virtual void addNewVar(std::filesystem::path FilePath, std::string VarType, std::string Var) const {
            
        }
        virtual void removeVar(std::filesystem::path FilePath, std::string SelectedVar) const {
            
        }*/
        virtual void readAll(std::filesystem::path FilePath) const {
            std::ifstream data(FilePath);
            if (data) {
                std::string line, Key, Value;
                std::vector<std::string> Keys;
                std::map<std::string, std::string> Values;
                bool foundVar = false;
                while (getline(data, line)) {
                    if (line.find(":")!= std::string::npos) {
                        size_t pos = line.find(":");
                        Key = line.substr(0, pos);
                        Value = line.substr(pos + 2); // Skip ": " after the key
                        Keys.push_back(Key);
                        Values[Key] = Value;
                    }
                }
                for (const auto& pair : Values) {
                    std::cout << pair.first << " ----- " << pair.second << std::endl;
                }
                data.close();
            }
            else {
            }
        }
};