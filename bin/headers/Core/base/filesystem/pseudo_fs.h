// Pseudo-FS header
// by SpaceOC
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