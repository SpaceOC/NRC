#ifndef NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#define NRC_BASE_FILESYSTEM_PSEUDO_FS_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

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
		NRFS* _nrfs = nullptr;
		std::string _currentPath = "./";
		size_t _currentDiskId = 0;

		bool folderExistsHelper(std::vector<std::string> path, FolderData* currentFolder);
		bool fileExistsHelper(std::vector<std::string> path, FolderData* currentFolder);

		int createFolderHelper(std::vector<std::string> path, FolderData* currentFolder, FolderData* oneFolderData = nullptr);
		int setFolderAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt);
		int renameFolderHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName);
		int deleteFolderHelper(std::vector<std::string> path, FolderData* currentFolder);
		int moveFolderHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const FolderData& oldFolderData, const std::string& oldPath);
		FolderData getFolderDataHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code);

		int createFileHelper(std::vector<std::string> path, FolderData* currentFolder, FileData* oneFileData = nullptr);
		int renameFileHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName);
		int deleteFileHelper(std::vector<std::string> path, FolderData* currentFolder);
		int moveFileHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const FileData& oldFileData, const std::string& oldPath);
		int setFileAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt);
		FileData getFileDataHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code);
	public:
		PseudoFS() = default;
		PseudoFS(PseudoFS&) = delete;
		PseudoFS(const PseudoFS&&) = delete;
		~PseudoFS();

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

		int createFolder(const std::string& path, size_t diskId, FolderData* oneFolderData = nullptr);
		int setFolderAtt(const std::string& path, size_t diskId, const std::string& what, std::any newAtt);
		int renameFolder(const std::string& path, size_t diskId, std::string newName);
		int deleteFolder(const std::string& path, size_t diskId);
		int moveFolder(const std::string& path, size_t diskId, const std::string& newPath, size_t anotherDiskId);
		FolderData getFolderData(const std::string& path, size_t diskId, int& code);

		int createFile(const std::string& path, size_t diskId, FileData* oneFileData = nullptr);
		int setFileAtt(const std::string& path, size_t diskId, const std::string& what, std::any newAtt);
		int renameFile(const std::string& path, size_t diskId, std::string newName);
		int deleteFile(const std::string& path, size_t diskId);
		int moveFile(const std::string& path, size_t diskId, const std::string& newPath, size_t anotherDiskId);
		FileData getFileData(const std::string& path, size_t diskId, int& code);

		void printAllHelper(const std::vector<std::shared_ptr<FolderData>>& folders, const std::string &path, bool includeHidden);
		void printAllHelper(const std::vector<std::shared_ptr<FolderData>>& folders, const std::string &path, bool includeHidden, std::string& str);
		void printAll(bool includeHidden, size_t diskId, const std::string& startPath = "./");
		void printAll(bool includeHidden, size_t diskId, const std::string& startPath, std::string& str);
		void showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level);
		void showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level, std::string& str);
		void showTree(bool includeHidden, size_t diskId, bool showFiles, const std::string& startPath = "./");
		void showTree(bool includeHidden, size_t diskId, bool showFiles, const std::string& startPath, std::string& str);

		NRFS* getNRFS();

		int changePath(const std::string& newPath, size_t diskId);
		int changeDirectory(const std::string& newDirectory, size_t diskId);
		int changeDisk(char diskLetter);
		bool folderExists(const std::string& path, size_t diskId);
		bool fileExists(const std::string& path, size_t diskId);
		bool isFile(const std::string& path);

		const std::string& currentPath();
		size_t currentDiskId() { return _currentDiskId; }

		std::string getFileFromPath(const std::string& path);
		std::string getFolderFromPath(const std::string& path);

		PseudoFS operator=(const PseudoFS& right) = delete;
	};

	extern PseudoFS* pseudoFS();
}

#endif