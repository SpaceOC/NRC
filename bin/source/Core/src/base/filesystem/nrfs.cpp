#include "Core/base/filesystem/nrfs.h"
#include <iostream>

int core::NRFSDisk::updateHelper(const std::vector<FolderData*>& folders) {
    int temp = 0;
    for (FolderData* folder : folders) {
        foldersSize++;
        filesSize += folder->files.size() - (folder->files.size() == 1 ? 0 : 1);
        temp += folder->files.size() - (folder->files.size() == 1 ? 0 : 1);
        temp += updateHelper(folder->folders);
    }
    return temp;
}

void core::NRFSDisk::update() {
    filesSize = 0;
    foldersSize = 0;
    diskSize = 0;
    if (folders.empty() && files.empty()) return;
    int temp = 0;
    filesSize += files.size() - (files.size() == 1 ? 0 : 1);
    temp += files.size() - (files.size() == 1 ? 0 : 1);
    this->diskSize = temp;
    this->diskSize += this->updateHelper(folders);
}

core::NRFS::NRFS() {
    this->root = new NRFSDisk();
    this->root->letter = 'C';
    this->root->update();
    this->disks.push_back(root);
}

core::NRFSDisk* core::NRFS::getRoot() {
    return root;
}

void core::NRFS::createDisk(char c) {
    for (auto d : disks) {
        if (d->letter == c)
            return;
    }
    core::NRFSDisk* disk = new NRFSDisk();
    disk->letter = c;
    disk->update();
    disks.push_back(disk);
};

void core::NRFS::renameDisk(char c, const std::string& name) {
    for (auto d : disks) {
        if (d->letter == c) {
            d->name = name;
            break;
        }
    }
}
