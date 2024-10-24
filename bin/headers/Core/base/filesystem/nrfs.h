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
#ifndef NRC_BASE_FILESYSTEM_NRFS_H_
#define NRC_BASE_FILESYSTEM_NRFS_H_
#include <string>
#include <vector>

#define NRFS_VERSION "0.1.1"
#define NRFS_VERSION_TYPE "alpha"

namespace core {
    // Contains all of the file's data.
    struct FileData {
        std::string name; // File name
        std::string content;
        int id;
        time_t dataCreate;
        time_t dataEdit;
        bool system; // Whether the file is a system file.
        bool hidden; // Whether the file is a hidden file.
        FileData* link = nullptr;
        std::string linkPath;
    };

    // Contains all folder data
    struct FolderData {
        std::string name; // Folder name
        int id;
        time_t dataCreate;
        time_t dataEdit;
        std::vector<FileData> files; // Contains the files that are in this folder.
        std::vector<FolderData> folders; // Contains the folders that are in this folder.
        bool system; // Whether the folder is a system folder.
        bool hidden; // Whether the folder is hidden.
        FolderData* link = nullptr;
        std::string linkPath;
    };

    class NRFSDisk {
        private:
            int diskSize;
            int foldersSize;
            int filesSize;
        public:
            const int& getDiskSize();
            const int& getFoldersSize();
            const int& getFilesSize();
            std::string directory;
            std::vector<FolderData> folders;
            std::vector<FileData> files;

            int updateHelper(const std::vector<FolderData>& folders);
            void update();
    };

    class NRFS {
        private:
            NRFSDisk root;
        public:
            NRFS();
            NRFSDisk& getRoot();
    };
}

#endif