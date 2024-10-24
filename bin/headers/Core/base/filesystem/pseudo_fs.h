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
	enum PseudoFSCodes {
		UNKNOWN_ERROR,
		OK = 1,

		NOT_FOUND,
		ALREADY_EXISTS,
		PATH_IS_EMPTY,
		THIS_LINK,
		BAD_PATH,

		JSON_PARSING_FAILED,
		IS_EMPTY,

		SMALL_SIZE
	};

	class PseudoFS {
		private:
			static inline NRFS nrfs;
			static inline int tempID = 0;
			static inline std::string currentPath = "./";

			static bool isFile(std::string path);

			static bool __folderExistsHelper(std::vector<std::string> path, FolderData& currentFolder);

			static nlohmann::json __savePFSHelper(std::vector<FolderData>& folders);
			static nlohmann::json __savePFSHelper(std::vector<FileData>& files);
			static std::vector<FolderData> __loadPFSHelperFolders(nlohmann::json folderJsonData);
			static std::vector<FileData> __loadPFSHelperFiles(nlohmann::json filesJsonData);
			static void __loadLinksFoldersAndFiles(FolderData& curFolder);

			static int __createFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const FolderData& oneFolderData = {});
			static int __setFolderAttHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& what, const std::any& newAtt);
			static int __renameFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& newName);
			static int __deleteFolderHelper(std::vector<std::string> path, FolderData& currentFolder);
			static int __moveFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const FolderData& oldFolderData, const std::string& oldPath);
			static FolderData __getFolderData(std::vector<std::string> path, FolderData& currentFolder, int& code);

			static int __createFileHelper(std::vector<std::string> path, FolderData& currentFolder, const FileData& oneFileData = {});
			static int __renameFileHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& newName);
			static int __deleteFileHelper(std::vector<std::string> path, FolderData& currentFolder);
			static int __moveFileHelper(std::vector<std::string> path, FolderData& currentFolder, const FileData& oldFileData, const std::string& oldPath);
			static int __setFileAttHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& what, const std::any& newAtt);
			static FileData __getFileData(std::vector<std::string> path, FolderData& currentFolder, int& code);
		public:
			static int createFolder(std::string path, const FolderData& oneFolderData = {});
			static int setFolderAtt(std::string path, std::string what, std::any newAtt);
			static int renameFolder(std::string path, std::string newName);
			static int deleteFolder(std::string path);
			static int moveFolder(std::string path, const std::string& newPath);
			static FolderData getFolderData(std::string path, int& code);

			static int createFile(std::string path, const FileData& oneFileData = {});
			static int setFileAtt(std::string path, std::string what, std::any newAtt);
			static int renameFile(std::string path, std::string newName);
			static int deleteFile(std::string path);
			static int moveFile(std::string path, const std::string& newPath);
			static FileData getFileData(std::string path, int& code);

            static void printAllHelper(const std::vector<FolderData>& folders, const std::string &path, bool includeHidden);
            static void printAll(bool includeHidden, std::string startPath = "./");
            static void showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level);
            static void showTree(bool includeHidden, bool showFiles, std::string startPath = "./");
            static void printDiskSize();

			static NRFS& getNRFS();

			static void savePFS();
			static int loadPFS();

			static int changePath(const std::string& newPath);
			static int changeDirectory(const std::string& newDirectory);
			static bool folderExists(const std::string& path);

			static const std::string getCurrentPath();

			static void init();
	};
}

#endif