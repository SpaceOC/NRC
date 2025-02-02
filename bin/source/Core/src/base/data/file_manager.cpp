#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include "Core/base/data/file_manager.h"

bool core::FileManager::fileExist(std::filesystem::path filePath) const {
	std::ifstream file(filePath, std::ios::in);
	if (file) {
		file.close();
		return true;
	}
	return false;
}

bool core::FileManager::folderExist(std::filesystem::path folderPath) const {
	return std::filesystem::is_directory(folderPath);
}

bool core::FileManager::folderIsEmpty(std::filesystem::path folderPath) const {
	return std::filesystem::is_empty(folderPath);
}

/*
bool core::FileManager::fileIsNotSystemFile(std::filesystem::path filePath) const {
	return (systemFiles.count(filePath) && systemFiles[filePath] == true);
}

bool core::FileManager::folderIsNotSystemFile(std::filesystem::path folderPath) const {
	return (systemFolders.count(folderPath) && systemFolders[folderPath]);
}
*/

//
void core::FileManager::createFile(const std::filesystem::path filePath) const {
	if (!fileExist(filePath)) {
		std::ofstream file(filePath);
		file.close();
	}
}

void core::FileManager::createFile(const std::filesystem::path filePath, std::string firstData) const {
	if (!fileExist(filePath)) {
		std::ofstream file(filePath);
		file << firstData << '\n';
		file.close();
	}
}

void core::FileManager::createFiles(const std::vector<std::filesystem::path> filePath) const {
	for (auto oneFile : filePath) {
		if (!fileExist(oneFile)) {
			std::ofstream file(oneFile);
			file.close();
		}
	}
}

void core::FileManager::deleteFile(const std::filesystem::path filePath) const {
	if (fileExist(filePath)) {
		std::filesystem::remove(filePath);
	}
}

void core::FileManager::renameFile(const std::filesystem::path filePath, const std::filesystem::path newFileName) const {
	if (fileExist(filePath)) { std::filesystem::rename(filePath, newFileName); }
}

std::string core::FileManager::readFile(const std::filesystem::path filePath) const {
	if (fileExist(filePath)) {
		std::ifstream file(filePath, std::ios::in);
		std::string line, temp;
		while (std::getline(file, line)) { temp += line; }
		file.close();
		return temp;
	}
	return "";
}

void core::FileManager::createFolder(const std::filesystem::path folderPath) const {
	if (!folderExist(folderPath)) { std::filesystem::create_directory(folderPath); }
}

void core::FileManager::createFolders(const std::vector<std::filesystem::path> folderPath) const {
	for (auto oneFolder : folderPath) {
		if (!folderExist(oneFolder)) { std::filesystem::create_directory(oneFolder); }
	}
}

void core::FileManager::deleteFolder(const std::filesystem::path folderPath) const {
	if (folderExist(folderPath) && folderIsEmpty(folderPath)) {
		std::filesystem::remove(folderPath);
	}
	if (folderExist(folderPath) && !folderIsEmpty(folderPath)) {
		std::filesystem::remove_all(folderPath);
	}
}

std::ifstream core::FileManager::getFileIfstream(std::filesystem::path filePath, std::ios_base::openmode mode) {
	if (fileExist(filePath)) { 
		std::ifstream file(filePath, mode); return file;
	}
	return {};
}

std::ofstream core::FileManager::getFileOfstream(std::filesystem::path filePath, std::ios_base::openmode mode) {
	if (fileExist(filePath)) { std::ofstream file(filePath, mode); return file; }
	return {};
}

std::fstream core::FileManager::getFileFstream(std::filesystem::path filePath, std::ios_base::openmode mode) {
	if (fileExist(filePath)) { std::fstream file(filePath, mode); return file; }
	return {};
}