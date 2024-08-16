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

struct fileData {
	std::string name;
	int id;
	time_t dataCreate;
	time_t dataEdit;
};

struct folderData {
	std::string name;
	int id;
	time_t dataCreate;
	time_t dataEdit;
};

class pseudoFSBase {
	private:
		
	public:
		void createFolder();
		void renameFolder();
		void deleteFolder();
		void teleportFolder();

		time_t getFolderDataCreate();
		time_t getFolderDataEdit();

		void createFile();
		void renameFile();
		void deleteFile();
		void teleportFile();

		time_t getFileDataCreate();
		time_t getFileDataEdit();
};
#endif