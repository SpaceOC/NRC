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
#ifndef NRC_BASE_FILESYSTEM_NRFS_H_
#define NRC_BASE_FILESYSTEM_NRFS_H_
#include <string>
#include <vector>

namespace core {
    struct fileData {
        std::string name, content;
        int id;
        time_t dataCreate;
        time_t dataEdit;
    };

    struct folderData {
        std::string name;
        int id;
        time_t dataCreate;
        time_t dataEdit;
        std::vector<fileData> files;
        std::vector<folderData> folders;
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
            std::vector<folderData> folders;
            std::vector<fileData> files;

            int updateHelper(const std::vector<folderData>& folders);
            void update();
    };

    class NRFS {
        private:
            NRFSDisk root;
        public:
            NRFS();
            void pushFiles(const std::vector<fileData>& files);

            void printAllHelper(const std::vector<folderData>& folders);
            void printAll();
            void printDiskSize();

            NRFSDisk& getRoot();
    };
}

#endif