#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>

// File Manager настоящей файловой системы
class fileManager {
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
            return std::filesystem::is_directory(folderPath);
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

        void createFile(const std::filesystem::path filePath) const {
            if (!fileExist(filePath)) {
                std::ofstream file(filePath);
                file.close();
            }
        }

        void createFile(const std::filesystem::path filePath, std::string firstData) const {
            if (!fileExist(filePath)) {
                std::ofstream file(filePath);
                file << firstData << '\n';
                file.close();
            }
        }

        void createFiles(const std::vector<std::filesystem::path> filePath) const {
            for (auto oneFile : filePath) {
                if (!fileExist(oneFile)) {
                    std::ofstream file(oneFile);
                    file.close();
                }
            }
        }

        void deleteFile(const std::filesystem::path filePath) const {
            if (fileExist(filePath)) {
                std::filesystem::remove(filePath);
            }
        }

        void renameFile(const std::filesystem::path filePath, const std::filesystem::path newFileName) const {
            if (fileExist(filePath)) { std::filesystem::rename(filePath, newFileName); }
        }

        std::string readFile(const std::filesystem::path filePath) const {
            std::string Temp;
            if (fileExist(filePath)) { 
                std::ifstream file(filePath, std::ios::in);
                std::string line;
                while (std::getline(file, line)) { Temp += line; }
                file.close();
            }
            else { Temp = "Null"; }
            return Temp;
        }

        void createFolder(const std::filesystem::path folderPath) const {
            if (!folderExist(folderPath)) { std::filesystem::create_directory(folderPath); }
        }

        void createFolders(const std::vector<std::filesystem::path> folderPath) const {
            for (auto oneFolder : folderPath) {
                if (!folderExist(oneFolder)) { std::filesystem::create_directory(oneFolder); }
            }
        }

        void deleteFolder(const std::filesystem::path folderPath) const {
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