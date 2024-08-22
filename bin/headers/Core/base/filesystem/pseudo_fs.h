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
// is not finished!
#pragma once
#ifndef NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#define NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Core/base/filesystem/nrfs.h"

namespace core {
	class pseudoFSBase {
		private:
			static inline NRFS nrfs;
			static inline int tempID = 0;
			std::vector<std::string> pathParser(std::string path);
			folderData searchFolderHelper(std::vector<std::string> path); // bad
			fileData searchFileHelper(std::vector<std::string> path); // bad
			bool isFile(std::string path);
			//bool exist(std::string path); not finished

			void __createFolderHelper(std::vector<std::string> path, std::vector<folderData>& folders);
			
			void __createFileHelper(std::vector<std::string> path, std::vector<folderData>& folders);
			void __setFileContentHelper(std::vector<std::string> path, std::vector<folderData>& folders, const std::string& content); // not working - The 'content' variable in 'file' doesn't change...? 
		public:
			void createFolder(std::string path);
			void renameFolder(std::string path, std::string newName);
			void deleteFolder(std::string path);
			//void moveFolder(std::string path); // its hard

			folderData getFolderData(std::string path); // not working?? maybe

			void createFile(std::string path);
			void setFileContent(std::string path, std::string content); // not working
			void renameFile(std::string path, std::string newName);
			void deleteFile(std::string path);
			//void moveFile(std::string path);  // its hard

			fileData getFileData(std::string path); // not working - returning nothing

			const NRFS& getNRFS();
	};
}

#endif