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
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include "Core/base/data/file_manager.h"

bool core::fileManager::fileExist(std::filesystem::path filePath) const {
	std::ifstream file(filePath, std::ios::in);
	if (file) {
		file.close();
		return true;
	}
	return false;
}

bool core::fileManager::folderExist(std::filesystem::path folderPath) const {
	return std::filesystem::is_directory(folderPath);
}

bool core::fileManager::folderIsEmpty(std::filesystem::path folderPath) const {
	return std::filesystem::is_empty(folderPath);
}

/*
bool core::fileManager::fileIsNotSystemFile(std::filesystem::path filePath) const {
	return (systemFiles.count(filePath) && systemFiles[filePath] == true);
}

bool core::fileManager::folderIsNotSystemFile(std::filesystem::path folderPath) const {
	return (systemFolders.count(folderPath) && systemFolders[folderPath]);
}
*/

//
void core::fileManager::createFile(const std::filesystem::path filePath) const {
	if (!fileExist(filePath)) {
		std::ofstream file(filePath);
		file.close();
	}
}

void core::fileManager::createFile(const std::filesystem::path filePath, std::string firstData) const {
	if (!fileExist(filePath)) {
		std::ofstream file(filePath);
		file << firstData << '\n';
		file.close();
	}
}

void core::fileManager::createFiles(const std::vector<std::filesystem::path> filePath) const {
	for (auto oneFile : filePath) {
		if (!fileExist(oneFile)) {
			std::ofstream file(oneFile);
			file.close();
		}
	}
}

void core::fileManager::deleteFile(const std::filesystem::path filePath) const {
	if (fileExist(filePath)) {
		std::filesystem::remove(filePath);
	}
}

void core::fileManager::renameFile(const std::filesystem::path filePath, const std::filesystem::path newFileName) const {
	if (fileExist(filePath)) { std::filesystem::rename(filePath, newFileName); }
}

std::string core::fileManager::readFile(const std::filesystem::path filePath) const {
	if (fileExist(filePath)) {
		std::ifstream file(filePath, std::ios::in);
		std::string line, temp;
		while (std::getline(file, line)) { temp += line; }
		file.close();
		return temp;
	}
	return "";
}

void core::fileManager::createFolder(const std::filesystem::path folderPath) const {
	if (!folderExist(folderPath)) { std::filesystem::create_directory(folderPath); }
}

void core::fileManager::createFolders(const std::vector<std::filesystem::path> folderPath) const {
	for (auto oneFolder : folderPath) {
		if (!folderExist(oneFolder)) { std::filesystem::create_directory(oneFolder); }
	}
}

void core::fileManager::deleteFolder(const std::filesystem::path folderPath) const {
	if (folderExist(folderPath) && folderIsEmpty(folderPath)) {
		std::filesystem::remove(folderPath);
	}
	if (folderExist(folderPath) && !folderIsEmpty(folderPath)) {
		std::filesystem::remove_all(folderPath);
	}
}

std::ifstream core::fileManager::getFileIfstream(std::filesystem::path filePath, std::ios_base::openmode mode) {
	if (fileExist(filePath)) { 
		std::ifstream file(filePath, mode); return file;
	}
}

std::ofstream core::fileManager::getFileOfstream(std::filesystem::path filePath, std::ios_base::openmode mode) {
	if (fileExist(filePath)) { std::ofstream file(filePath, mode); return file; }
}

std::fstream core::fileManager::getFileFstream(std::filesystem::path filePath, std::ios_base::openmode mode) {
	if (fileExist(filePath)) { std::fstream file(filePath, mode); return file; }
}