#pragma once
#ifndef FILE_MANAGER
#define FILE_MANAGER
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <fstream>

class fileManager {
	private:
		static inline std::map<std::filesystem::path, bool> systemFiles, systemFolders;
	public:
		bool fileExist(std::filesystem::path filePath) const;
		bool folderExist(std::filesystem::path folderPath) const;
		bool folderIsEmpty(std::filesystem::path folderPath) const;
		//bool fileIsNotSystemFile(std::filesystem::path filePath) const;
		//bool folderIsNotSystemFile(std::filesystem::path folderPath) const;
		void createFile(std::filesystem::path filePath) const;
		void createFile(std::filesystem::path filePath, std::string firstData) const;
		void createFiles(std::vector<std::filesystem::path> filePath) const;
		void deleteFile(std::filesystem::path filePath) const;
		void renameFile(std::filesystem::path filePath, std::filesystem::path newFileName) const;
		std::string readFile(std::filesystem::path filePath) const;
		void createFolder(std::filesystem::path folderPath) const;
		void createFolders(std::vector<std::filesystem::path> folderPath) const;
		void deleteFolder(std::filesystem::path folderPath) const;

		std::ifstream getFileIfstream(std::filesystem::path filePath, std::ios_base::openmode mode = std::ios::in);
		std::ofstream getFileOfstream(std::filesystem::path filePath, std::ios_base::openmode mode = std::ios::out);
		std::fstream getFileFstream(std::filesystem::path filePath, std::ios_base::openmode mode = std::ios::out | std::ios::in);
};
#endif