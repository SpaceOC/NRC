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
#pragma once
#ifndef NRC_BASE_DATA_FILE_MANAGER_H_
#define NRC_BASE_DATA_FILE_MANAGER_H_
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <fstream>

namespace core {
	class fileManager {
		private:
			static inline std::map<std::filesystem::path, bool> systemFiles, systemFolders;
		public:
			bool fileExist(std::filesystem::path filePath) const;
			bool folderExist(std::filesystem::path folderPath) const;
			bool folderIsEmpty(std::filesystem::path folderPath) const;
			// 
			// bool fileIsNotSystemFile(std::filesystem::path filePath) const;
			// bool folderIsNotSystemFile(std::filesystem::path folderPath) const;

			//
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
}

#endif