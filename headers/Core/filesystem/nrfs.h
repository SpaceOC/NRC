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
		std::string name; // File name
		std::string content;
		int id; // File id
		time_t timeCreate;
		time_t timeEdit;
		bool system; // Whether the file is a system file.
		bool hidden; // Whether the file is a hidden file.
		FileData* link = nullptr;
		std::string linkPath = "";
		User* owner = nullptr; // File owner (если owner равен NULL, то файл общий (если конечно system не равен true))
		std::string ownerUsername = "";

		friend std::ostream& operator<<(std::ostream& os, FileData const& fd) {
			return os << "{ Name: " << fd.name + ", Id: " << fd.id <<
				", Content: \"" << fd.content << "\"" <<
				", Time Create: " << fd.timeCreate << ", Time Edit: " << fd.timeEdit <<
				", System: " << std::boolalpha << fd.system << ", Hidden: " << fd.hidden <<
				", Is link: " << (fd.link != NULL) << ", Link Path: " << fd.linkPath <<
				", File owner: " << (fd.owner == NULL ? (fd.system ? "[ SYSTEM ]" : " [ NONE ]") : fd.owner->getUsername()) << " }";
		}

		nlohmann::json buildJSON();
	};

	// Contains all folder data
	struct FolderData {
		std::string name; // Folder name
		int id;
		time_t timeCreate;
		time_t timeEdit;
		std::vector<std::shared_ptr<FileData>> files; // Contains the files that are in this folder.
		std::vector<std::shared_ptr<FolderData>> folders; // Contains the folders that are in this folder.
		bool system; // Whether the folder is a system folder.
		bool hidden; // Whether the folder is hidden.
		FolderData* link = nullptr;
		std::string linkPath = "";
		User* owner = nullptr; // Folder owner
		std::string ownerUsername = "";

		friend std::ostream& operator<<(std::ostream& os, FolderData const& fd) {
			return os << "{ Name: " << fd.name + ", Id: " << fd.id <<
				", Time Create: " << fd.timeCreate << ", Time Edit: " << fd.timeEdit <<
				", Files: " << (fd.files.size() == 0 ? 0 : (fd.files.size() == 1 ? 1 : fd.files.size() - 1)) <<
				", Folders: " << (fd.folders.size() == 0 ? 0 : (fd.folders.size() == 1 ? 1 : fd.folders.size() - 1)) <<
				", System: " << std::boolalpha << fd.system << ", Hidden: " << fd.hidden <<
				", Is link: " << (fd.link != NULL) << ", Link Path: " << fd.linkPath <<
				", Folder owner: " << (fd.owner == NULL ? (fd.system ? "[ SYSTEM ]" : " [ NONE ]") : fd.owner->getUsername()) << " }";
		}

		nlohmann::json buildJSON();
	};

	class NRFS;
	class PseudoFS;

	class NRFSDisk {
		private:
			int diskSize;
			int foldersSize;
			int filesSize;

			friend class NRFS;
			friend class PseudoFS;
		protected:
			std::string name;
			char letter;
			std::vector<std::shared_ptr<FolderData>> folders;
			std::vector<std::shared_ptr<FileData>> files;
		public:
			int getDiskSize() { this->update(); return (foldersSize + filesSize); }
			int getFoldersSize() { this->update(); return foldersSize; }
			int getFilesSize() { this->update(); return filesSize; }
			std::string getName() { return name; }
			char getLetter() { return letter; }
			const std::vector<std::shared_ptr<FolderData>> getFolders() { return folders; }
			const std::vector<std::shared_ptr<FileData>> getFiles() { return files; }

			int updateHelper(const std::vector<std::shared_ptr<FolderData>>& folders);
			void update();

			nlohmann::json buildJSON();
			void loadData(const nlohmann::json& j);
	};

	class NRFS {
		private:
			friend class PseudoFS;
		protected:
			NRFSDisk* root; // Main disk
			std::vector<std::shared_ptr<NRFSDisk>> disks; // All disks (including main disk)
			bool isLoadedFromFile;
		public:
			NRFS();
			NRFSDisk* getRoot();
			const std::vector<std::shared_ptr<NRFSDisk>> getDisks() { return disks; }
			void createDisk(char c);
			void renameDisk(char c, const std::string& name);
			void deleteDisk(char c);
			size_t getDiskFromLetter(char c);

			void saveData();
			void loadData();
	};
}

#endif