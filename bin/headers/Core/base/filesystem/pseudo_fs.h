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
#ifndef NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#define NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include "Core/base/filesystem/nrfs.h"

namespace core {
	class PseudoFS {
		private:
			static inline NRFS nrfs;
			static inline int tempID = 0;
			static inline std::string currentPath = "./";

			bool isFile(std::string path);

			bool __folderExistsHelper(std::vector<std::string> path, FolderData& currentFolder);

			nlohmann::json __savePFSHelper(std::vector<FolderData>& folders);
			nlohmann::json __savePFSHelper(std::vector<FileData>& files);
			std::vector<FolderData> __loadPFSHelperFolders(nlohmann::json folderJsonData);
			FolderData __loadPFSHelperSingleFolder(nlohmann::json folderJsonData);
			std::vector<FileData> __loadPFSHelperFiles(nlohmann::json filesJsonData);

			bool __createFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const FolderData& oneFolderData = {});
			bool __renameFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& newName);
			bool __deleteFolderHelper(std::vector<std::string> path, FolderData& currentFolder);
			bool __moveFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const FolderData& oldFolderData, const std::string& oldPath);
			FolderData __getFolderData(std::vector<std::string> path, FolderData& currentFolder);

			bool __createFileHelper(std::vector<std::string> path, FolderData& currentFolder, const FileData& oneFileData = {});
			bool __renameFileHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& newName);
			bool __deleteFileHelper(std::vector<std::string> path, FolderData& currentFolder);
			bool __moveFileHelper(std::vector<std::string> path, FolderData& currentFolder, const FileData& oldFileData, const std::string& oldPath);
			bool __setFileAttHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& what, const std::any& newAtt);
			FileData __getFileData(std::vector<std::string> path, FolderData& currentFolder);
		public:
			void createFolder(std::string path, const FolderData& oneFolderData = {}, bool silent = false);
			void renameFolder(std::string path, std::string newName, bool silent = false);
			void deleteFolder(std::string path, bool silent = false);
			void moveFolder(std::string path, const std::string& newPath, bool silent = false);
			FolderData getFolderData(std::string path, bool silent = false);

			void createFile(std::string path, const FileData& oneFileData = {}, bool silent = false);
			void setFileAtt(std::string path, std::string what, std::any newAtt, bool silent = false);
			void renameFile(std::string path, std::string newName, bool silent = false);
			void deleteFile(std::string path, bool silent = false);
			void moveFile(std::string path, const std::string& newPath, bool silent = false);
			FileData getFileData(std::string path, bool silent = false);

            void printAllHelper(const std::vector<FolderData>& folders, const std::string &path, bool includeHidden);
            void printAll(bool includeHidden, std::string startPath = "./");
            void showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level);
            void showTree(bool includeHidden, bool showFiles, std::string startPath = "./");
            void printDiskSize();

			NRFS& getNRFS();

			void savePFS();
			void loadPFS();

			bool changePath(const std::string& newPath);
			bool changeDirectory(const std::string& newDirectory);
			bool folderExists(const std::string& path);

			const std::string getCurrentPath();

			void init();
	};
}

#endif