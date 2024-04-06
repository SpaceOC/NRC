#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Sorry. Я oblenilsa делать pseudoFS :skull:
class pseudoFSBase {
    private:
        static inline std::string currentDirectory; // Текущая директорая, где находится пользователь
        static inline std::map<std::string, std::string> folders;
        static inline std::map<std::string, std::string> files;
    public:
        bool fileExist(std::string file) {
            return files.count(file);
        }

        bool folderExist(std::string folder) {
            return folders.count(folder);
        }
        
        std::string fileType(std::string type) {
            std::vector<std::string> availableTypes = {"SYS", "CONFIG", "TXT", "LOG", "DATA", "JSON"};
            std::string temp = "";
            if (type == "SYS") { return ".sys"; }
            if (type == "CONFIG") { return ".config"; }
            if (type == "TXT") { return ".txt"; }
            if (type == "LOG") { return ".log"; }
            if (type == "DATA") { return ".data"; }
            if (type == "JSON") { return ".json"; }
            for (auto& element : availableTypes) {
                if (type != element) {
                    temp = ".error";
                }
            }
            return temp;
        }

        void createFile(std::string path, std::string file, std::string type) {
            if (!fileExist(file)) { files[file] = file + type; }
        }
        void createFiles(std::vector<std::string> files, std::vector<std::string> types) {

        }
        void renameFile(std::string file, std::string newFileName) {
            std::string Temp;
            if (fileExist(file)) {
                Temp = files[file];
            }
        }
        void deleteFile() {

        }

        void createFolder(std::string folderName) {
            folders[folderName] = "";
        }
        void createFolders() {

        }
        void renameFolder() {

        }
        void deleteFolder() {
            
        }
};