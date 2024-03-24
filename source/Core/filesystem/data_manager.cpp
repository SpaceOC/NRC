#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>

// Data Manager настоящей файловой системы
class DataManager {
    private:
        static inline std::map<std::filesystem::path, bool> systemFiles, systemFolders;
    public:
        bool fileExist(std::filesystem::path filePath) const {
            std::ifstream file(filePath, std::ios::in);
            if (file) {
                file.close();
                return true;
            }
            else { return false; }
        }

        bool folderExist(std::filesystem::path folderPath) const {
            return std::filesystem::exists(folderPath);
        }

        bool folderIsEmpty(std::filesystem::path folderPath) const {
            return std::filesystem::is_empty(folderPath);
        }

        bool fileIsNotSystemFile(std::filesystem::path filePath) const {
            if (systemFiles.count(filePath) && systemFiles[filePath] == true) {
                return false;
            }
            else {
                return true;
            }
        }

        bool folderIsNotSystemFile(std::filesystem::path folderPath) const {
            if (systemFolders.count(folderPath) && systemFolders[folderPath] == true) {
                return false;
            }
            else {
                return true;
            }
        }

        void createFile(std::filesystem::path filePath) const {
            if (!fileExist(filePath)) {
                std::ofstream file(filePath);
                file.close();
            }
        }

        void createFile(std::filesystem::path filePath, std::string firstData) const {
            if (!fileExist(filePath)) {
                std::ofstream file(filePath);
                file << firstData << '\n';
                file.close();
            }
        }

        void createFiles(std::vector<std::filesystem::path> filePath) const {
            for (auto oneFile : filePath) {
                if (!fileExist(oneFile)) {
                    std::ofstream file(oneFile);
                    file.close();
                }
            }
        }

        void deleteFile(std::filesystem::path filePath) const {
            if (fileExist(filePath)) {
                std::filesystem::remove(filePath);
            }
        }

        void renameFile(std::filesystem::path filePath, std::filesystem::path newFileName) const {
            if (fileExist(filePath)) { std::filesystem::rename(filePath, newFileName); }
        }

        virtual void readFile(std::filesystem::path filePath) const {
            if (fileExist(filePath)) { 
                std::ifstream file(filePath, std::ios::in);
                std::string line;
                while (std::getline(file, line)) { std::cout << line << std::endl; }
            }
        }

        void createFolder(std::filesystem::path folderPath) const {
            if (!folderExist(folderPath)) { std::filesystem::create_directory(folderPath); }
        }

        void createFolders(std::vector<std::filesystem::path> folderPath) const {
            for (auto oneFolder : folderPath) {
                if (!folderExist(oneFolder)) { std::filesystem::create_directory(oneFolder); }
            }
        }

        void deleteFolder(std::filesystem::path folderPath) const {
            if (folderExist(folderPath) && folderIsEmpty(folderPath)) {
                std::filesystem::remove(folderPath);
            }
            if (folderExist(folderPath) && !folderIsEmpty(folderPath)) {
                std::filesystem::remove_all(folderPath);
            }
        }
        
        /*void renameFolder() {

        }*/
};