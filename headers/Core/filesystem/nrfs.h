#ifndef NRC_BASE_FILESYSTEM_NRFS_H_
#define NRC_BASE_FILESYSTEM_NRFS_H_
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "Core/users/user.h"

#define NRFS_VERSION "0.1.8"
#define NRFS_VERSION_TYPE "alpha"

namespace core {
	// Contains all of the file's data.
	struct FileData {
		// File name
		std::string name;
		// File content
		std::string content;
		time_t createTime;
		time_t lastEditTime;
		// Whether the file is a system file.
		bool system;
		// Whether the file is a hidden file.
		bool hidden;
		// link
		FileData* realFilePointer = nullptr;
		// real file path
		std::string realFilePath = "";
		// File owner (если owner равен NULL, то файл общий (если конечно system не равен true))
		User* owner = nullptr;
		std::string ownerUsername = "";

		friend std::ostream& operator<<(std::ostream& os, FileData const& fd) {
			return os << "{ Name: " << fd.name + ", Content: \"" << fd.content << "\"" <<
				", Time Create: " << fd.createTime << ", Time Edit: " << fd.lastEditTime <<
				", System: " << std::boolalpha << fd.system << ", Hidden: " << fd.hidden <<
				", Is link: " << (fd.realFilePointer != NULL) << ", Link Path: " << fd.realFilePath <<
				", File owner: " << (fd.owner == NULL ? (fd.system ? "[ SYSTEM ]" : " [ NONE ]") : fd.owner->getUsername()) << " }";
		}

		nlohmann::json buildJSON();
	};

	// Contains all folder data
	struct FolderData {
		// Folder name
		std::string name;
		time_t createTime;
		time_t lastEditTime;
		// Contains the files that are in this folder.
		std::vector<std::shared_ptr<FileData>> files;
		// Contains the folders that are in this folder.
		std::vector<std::shared_ptr<FolderData>> folders;
		// Whether the folder is a system folder.
		bool system;
		// Whether the folder is hidden.
		bool hidden;
		// link
		FolderData* realFolderPointer = nullptr;
		// real file path
		std::string realFolderPath = "";
		// Folder owner
		User* owner = nullptr;
		std::string ownerUsername = "";
		FolderData* parent = nullptr;

		friend std::ostream& operator<<(std::ostream& os, FolderData const& fd) {
			return os << "{ Name: " << fd.name + ", Time Create: " << fd.createTime <<
				", Time Edit: " << fd.lastEditTime <<
				", Files: " << (fd.files.size() == 0 ? 0 : (fd.files.size() == 1 ? 1 : fd.files.size() - 1)) <<
				", Folders: " << (fd.folders.size() == 0 ? 0 : (fd.folders.size() == 1 ? 1 : fd.folders.size() - 1)) <<
				", System: " << std::boolalpha << fd.system << ", Hidden: " << fd.hidden <<
				", Is link: " << (fd.realFolderPointer != NULL) << ", Link Path: " << fd.realFolderPath <<
				", Folder owner: " << (fd.owner == NULL ? (fd.system ? "[ SYSTEM ]" : " [ NONE ]") : fd.owner->getUsername()) << " }";
		}

		nlohmann::json buildJSON();
	};

	class NRFS;
	class PseudoFS;

	class NRFSDisk {
	private:
		int _foldersSize;
		int _filesSize;

		friend class NRFS;
		friend class PseudoFS;
	protected:
		std::string name;
		char letter;
		std::vector<std::shared_ptr<FolderData>> folders;
		std::vector<std::shared_ptr<FileData>> files;
	public:
		int getDiskSize() { this->update(); return (_foldersSize + _filesSize); }
		int getFoldersSize() { this->update(); return _foldersSize; }
		int getFilesSize() { this->update(); return _filesSize; }
		const std::string& getName() { return name; }
		char getLetter() { return letter; }
		const std::vector<std::shared_ptr<FolderData>>& getFolders() { return folders; }
		const std::vector<std::shared_ptr<FileData>>& getFiles() { return files; }

		void updateHelper(const std::vector<std::shared_ptr<FolderData>>& folders);
		void update();

		nlohmann::json buildJSON();
		void loadData(const nlohmann::json& j);
	};

	class NRFS {
	private:
		friend class PseudoFS;
	protected:
		// Main disk
		NRFSDisk* root;
		 // All disks (including main disk)
		std::vector<std::shared_ptr<NRFSDisk>> disks;
		bool isLoadedFromFile = false;
	public:
		NRFS();

		NRFSDisk* getRoot();
		const std::vector<std::shared_ptr<NRFSDisk>>& getDisks() { return disks; }
		void createDisk(char c);
		void renameDisk(char c, const std::string& name);
		void deleteDisk(char c);
		size_t getDiskFromLetter(char c);

		void saveData();
		void loadData();
	};
}

#endif