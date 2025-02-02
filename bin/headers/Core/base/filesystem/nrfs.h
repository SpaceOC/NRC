#ifndef NRC_BASE_FILESYSTEM_NRFS_H_
#define NRC_BASE_FILESYSTEM_NRFS_H_
#include <string>
#include <vector>
#include "Core/base/users/user.h"

#define NRFS_VERSION "0.1.7"
#define NRFS_VERSION_TYPE "alpha"

namespace core {
    // Contains all of the file's data.
    struct FileData {
        std::string name; // File name
        std::string content;
        int id; // File id
        time_t dataCreate;
        time_t dataEdit;
        bool system; // Whether the file is a system file.
        bool hidden; // Whether the file is a hidden file.
        FileData* link = nullptr;
        std::string linkPath = "";
        User* owner = nullptr; // File owner (если owner равен NULL, то файл общий (если конечно system не равен true))

        friend std::ostream& operator<<(std::ostream& os, FileData const& fd) {
            return os << "{ Name: " << fd.name + ", Id: " << fd.id <<
                ", Content: \"" << fd.content << "\"" <<
                ", Data Create: " << fd.dataCreate << ", Data Edit: " << fd.dataEdit <<
                ", System: " << std::boolalpha << fd.system << ", Hidden: " << fd.hidden <<
                ", Is link: " << (fd.link != NULL) << ", Link Path: " << fd.linkPath <<
                ", File owner: " << (fd.owner == NULL ? (fd.system ? "[ SYSTEM ]" : " [ NONE ]") : fd.owner->getUsername()) << " }";
        }
    };

    // Contains all folder data
    struct FolderData {
        std::string name; // Folder name
        int id;
        time_t dataCreate;
        time_t dataEdit;
        std::vector<FileData*> files; // Contains the files that are in this folder.
        std::vector<FolderData*> folders; // Contains the folders that are in this folder.
        bool system; // Whether the folder is a system folder.
        bool hidden; // Whether the folder is hidden.
        FolderData* link = nullptr;
        std::string linkPath = "";
        User* owner = nullptr; // Folder owner

        friend std::ostream& operator<<(std::ostream& os, FolderData const& fd) {
            return os << "{ Name: " << fd.name + ", Id: " << fd.id <<
                ", Data Create: " << fd.dataCreate << ", Data Edit: " << fd.dataEdit <<
                ", Files: " << (fd.files.size() == 1 ? 1 : fd.files.size() - 1) <<
                ", Folders: " << (fd.folders.size() == 1 ? 1 : fd.folders.size() - 1) <<
                ", System: " << std::boolalpha << fd.system << ", Hidden: " << fd.hidden <<
                ", Is link: " << (fd.link != NULL) << ", Link Path: " << fd.linkPath <<
                ", Folder owner: " << (fd.owner == NULL ? (fd.system ? "[ SYSTEM ]" : " [ NONE ]") : fd.owner->getUsername()) << " }";
        }
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
            std::vector<FolderData*> folders;
            std::vector<FileData*> files;
        public:
            int getDiskSize() { this->update(); return (foldersSize + filesSize); }
            int getFoldersSize() { this->update(); return foldersSize; }
            int getFilesSize() { this->update(); return filesSize; }
            std::string getName() { return name; }
            char getLetter() { return letter; }
            const std::vector<FolderData*> getFolders() { return folders; }
            const std::vector<FileData*> getFiles() { return files; }

            int updateHelper(const std::vector<FolderData*>& folders);
            void update();
    };

    class NRFS {
        private:
            friend class PseudoFS;
        protected:
            NRFSDisk* root; // Main disk
            std::vector<NRFSDisk*> disks; // All disks (including main disk (position 0))
        public:
            NRFS();
            NRFSDisk* getRoot();
            const std::vector<NRFSDisk*> getDisks() { return disks; }
            void createDisk(char c);
            void renameDisk(char c, const std::string& name);
            //void deleteDisk(char c);
    };
}

#endif