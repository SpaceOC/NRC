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

#include "Core/base/filesystem/nrfs.h"
#include <iostream>

const int& core::NRFSDisk::getFilesSize() {
    this->update();
    return this->filesSize;
}

const int& core::NRFSDisk::getFoldersSize() {
    this->update();
    return this->foldersSize;
}

const int& core::NRFSDisk::getDiskSize() {
    this->update();
    return this->diskSize;
}

int core::NRFSDisk::updateHelper(const std::vector<FolderData>& folders) {
    int temp = 0;
    for (const FolderData& folder : folders) {
        foldersSize++;
        filesSize += folder.files.size() - (folder.files.size() == 1 ? 0 : 1);
        temp += folder.files.size() - (folder.files.size() == 1 ? 0 : 1) + 1;
        temp += updateHelper(folder.folders);
    }
    return temp;
}

void core::NRFSDisk::update() {
    filesSize = 0;
    foldersSize = 0;
    int temp = 0;
    filesSize += files.size() - (files.size() == 1 ? 0 : 1);
    temp += files.size() - (files.size() == 1 ? 0 : 1);
    this->diskSize = temp;
    this->diskSize += this->updateHelper(folders);
}

core::NRFS::NRFS() {
    this->root = NRFSDisk();
    this->root.directory = ".";
    this->root.update();
}

core::NRFSDisk& core::NRFS::getRoot() {
    return root;
}
