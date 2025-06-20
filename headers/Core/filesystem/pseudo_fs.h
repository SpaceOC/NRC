#ifndef NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#define NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include "Core/settings.h"

namespace core {
	class NRFS;
	struct FileData;
	struct FolderData;

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

	std::string pseudoFSCodesS(int t);

	class PseudoFS {
		private:
			NRFS* nrfs;
			std::string currentPath = "./";
			size_t curDisk = 0;

			bool __folderExistsHelper(std::vector<std::string> path, FolderData* currentFolder);
			bool __fileExistsHelper(std::vector<std::string> path, FolderData* currentFolder);

			int __createFolderHelper(std::vector<std::string> path, FolderData* currentFolder, FolderData* oneFolderData = nullptr);
			int __setFolderAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt);
			int __renameFolderHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName);
			int __deleteFolderHelper(std::vector<std::string> path, FolderData* currentFolder);
			int __moveFolderHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const FolderData& oldFolderData, const std::string& oldPath);
			FolderData __getFolderData(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code);

			int __createFileHelper(std::vector<std::string> path, FolderData* currentFolder, FileData* oneFileData = nullptr);
			int __renameFileHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName);
			int __deleteFileHelper(std::vector<std::string> path, FolderData* currentFolder);
			int __moveFileHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const FileData& oldFileData, const std::string& oldPath);
			int __setFileAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt);
			FileData __getFileData(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code);
		public:
			PseudoFS() = default;
			PseudoFS(PseudoFS&) = delete;
			PseudoFS(const PseudoFS&&) = delete;
			bool isFile(std::string path);

			int createFolder(std::string path, size_t diskId, FolderData* oneFolderData = nullptr);
			int setFolderAtt(std::string path, size_t diskId, std::string what, std::any newAtt);
			int renameFolder(std::string path, size_t diskId, std::string newName);
			int deleteFolder(std::string path, size_t diskId);
			int moveFolder(std::string path, size_t diskId, const std::string& newPath, size_t anotherDiskId);
			FolderData getFolderData(std::string path, size_t diskId, int& code);

			int createFile(std::string path, size_t diskId, FileData* oneFileData = nullptr);
			int setFileAtt(std::string path, size_t diskId, std::string what, std::any newAtt);
			int renameFile(std::string path, size_t diskId, std::string newName);
			int deleteFile(std::string path, size_t diskId);
			int moveFile(std::string path, size_t diskId, const std::string& newPath, size_t anotherDiskId);
			FileData getFileData(std::string path, size_t diskId, int& code);

			void printAllHelper(const std::vector<std::shared_ptr<FolderData>>& folders, const std::string &path, bool includeHidden);
			void printAllHelper(const std::vector<std::shared_ptr<FolderData>>& folders, const std::string &path, bool includeHidden, std::string& str);
			void printAll(bool includeHidden, size_t diskId, std::string startPath = "./");
			void printAll(bool includeHidden, size_t diskId, std::string startPath, std::string& str);
			void showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level);
			void showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level, std::string& str);
			void showTree(bool includeHidden, size_t diskId, bool showFiles, std::string startPath = "./");
			void showTree(bool includeHidden, size_t diskId, bool showFiles, std::string startPath, std::string& str);

			NRFS* getNRFS();

			int changePath(const std::string& newPath, size_t diskId);
			int changeDirectory(const std::string& newDirectory, size_t diskId);
			int changeDisk(char diskLetter);
			bool folderExists(const std::string& path, size_t diskId);
			bool fileExists(const std::string& path, size_t diskId);

			const std::string getCurrentPath();
			size_t getCurDiskId() { return curDisk; }

			std::string getFileFromPath(const std::string& path);
			std::string getFolderFromPath(const std::string& path);

			/**
			 * First step of PFS initialization
			 * - Initializes NRFS
			 * - Creates the home folder and the hello.txt file (if they do not exist)
			 */
			void init();

			/**
			 * The second step of PFS initialization
			 * - Finds objects that have a non-empty linkPath variable, finds and converts these objects into links
			 */
			void postInit();
	};

	extern PseudoFS* pseudoFS();
}

#endif