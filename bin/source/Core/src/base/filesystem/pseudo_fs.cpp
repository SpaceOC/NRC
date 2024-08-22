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
#include "Core/base/filesystem/pseudo_fs.h"

std::vector<std::string> core::pseudoFSBase::pathParser(std::string path) {
    std::string folder;
    std::vector<std::string> temp;
    for (const char& letter : (path + '/')) {
        if (letter != '/')
            folder += letter;
        else {
            temp.push_back(folder);
            folder = "";
        }
    }
    return temp;
}

bool core::pseudoFSBase::isFile(std::string path) {
    std::string file = path.substr(path.rfind("/") + 1, path.length());
    if (file.rfind(".") == 1) return false;
    return true;
}

void core::pseudoFSBase::__createFolderHelper(std::vector<std::string> path, std::vector<folderData>& folders) {
    std::vector<folderData>* currentFolder = &folders;
    folderData* pastFolder = nullptr;
    int newID = 0;
    while (newID < path.size()) {
        bool found = false;
        for (folderData& folder : *currentFolder) {
            if (path[newID] == folder.name) {
                found = true;
                pastFolder = &folder;
                currentFolder = &(folder.folders);
                break;
            }
        }

        if (!found) {
            currentFolder->push_back({path[newID], tempID, 1, 1, {}, {}});
            tempID++;
            nrfs.getRoot().update();
            pastFolder = &(*currentFolder).back();
            currentFolder = &pastFolder->folders;
        }

        newID++;
    }
}

void core::pseudoFSBase::__createFileHelper(std::vector<std::string> path, std::vector<folderData>& folders) {
    std::vector<folderData>* currentFolders = &folders;
    folderData* currentFolder = nullptr;
    int newID = 0;
    while (newID < path.size() - 1) {
        bool found = false;
        for (folderData& folder : *currentFolders) {
            if (path[newID] == folder.name && (newID + 1) < path.size()) {
                currentFolders = &(folder.folders);
                currentFolder = &(folder);
                found = true;
                break;
            }
        }

        if (!found) {
            currentFolder = &currentFolders->back();
            tempID++;
            nrfs.getRoot().update();
        }
        
        newID++;
    }

    if (currentFolder != nullptr) {
        currentFolder->files.push_back({path.back(), "", tempID, 0, 0});
        tempID++;
        nrfs.getRoot().update();
    }
}

void core::pseudoFSBase::__setFileContentHelper(std::vector<std::string> path, std::vector<folderData>& folders, const std::string& content) {
    std::vector<folderData>* currentFolders = &folders;
    folderData* currentFolder = nullptr;
    int newID = 0;
    while (newID < path.size() - 1) {
        bool found = false;
        for (folderData& folder : *currentFolders) {
            if (path[newID] == folder.name && (newID + 1) < path.size()) {
                currentFolders = &(folder.folders);
                currentFolder = &(folder);
                found = true;
                break;
            }
        }

        if (!found) {
            currentFolder = &currentFolders->back();
            nrfs.getRoot().update();
        }
        
        newID++;
    }

    if (currentFolder != nullptr) {
        for (fileData& file : currentFolder->files) {
            if (file.name == path.back()) {
                // ???
                file.content = content;
            }
        }
        nrfs.getRoot().update();
    }
}

core::folderData core::pseudoFSBase::searchFolderHelper(std::vector<std::string> path) {
    std::vector<folderData>* currentFolders = &(nrfs.getRoot().folders);
    folderData* currentFolder = nullptr;
    int newID = 0;
    while (newID < path.size() - 1) {
        bool found = false;
        for (folderData& folder : *currentFolders) {
            if (path[newID] == folder.name && (newID + 1) < path.size()) {
                currentFolders = &(folder.folders);
                currentFolder = &(folder);
                found = true;
                break;
            }
        }
        if (!found) {
            currentFolder = &currentFolders->back();
            nrfs.getRoot().update();
        }
        newID++;
    }

    if (currentFolder != nullptr) {
        return *currentFolder;
    }
    return {};
}

core::fileData core::pseudoFSBase::searchFileHelper(std::vector<std::string> path) {
    std::vector<folderData>* currentFolders = &(nrfs.getRoot().folders);
    folderData* currentFolder = nullptr;
    int newID = 0;
    while (newID < path.size() - 1) {
        bool found = false;
        for (folderData& folder : *currentFolders) {
            if (path[newID] == folder.name && (newID + 1) < path.size()) {
                currentFolders = &(folder.folders);
                currentFolder = &(folder);
                found = true;
                break;
            }
        }
        if (!found) {
            currentFolder = &currentFolders->back();
            nrfs.getRoot().update();
        }
        newID++;
    }

    if (currentFolder != nullptr) {
        for (fileData& file : currentFolder->files) {
            if (file.name == path.back())
                return file;
        }
    }
    return {};
}

void core::pseudoFSBase::createFolder(std::string path) {
    std::vector<std::string> parsedPath = pathParser(path);
    if (parsedPath.size() == 2) {
        nrfs.getRoot().folders.push_back({parsedPath[1], tempID, 1, 1, {}, {}});
        nrfs.getRoot().update();
        tempID++;
    }
    else {
        __createFolderHelper(parsedPath, nrfs.getRoot().folders);
    }
}

core::folderData core::pseudoFSBase::getFolderData(std::string path) {
    return searchFolderHelper(pathParser(path));
}

void core::pseudoFSBase::createFile(std::string path) {
    std::vector<std::string> parsedPath = pathParser(path);
    if (parsedPath.size() == 2) {
        nrfs.getRoot().files.push_back({parsedPath[1], "", tempID, 1, 1});
        nrfs.getRoot().update();
        tempID++;
    }
    else {
        __createFileHelper(parsedPath, nrfs.getRoot().folders);
    }
}

void core::pseudoFSBase::setFileContent(std::string path, std::string content) {
    std::vector<std::string> parsedPath = pathParser(path);
    if (parsedPath.size() == 2) {
        for (fileData& file : nrfs.getRoot().files) {
            if (file.name == parsedPath[1]) {
                file.content = content;
            }
        }
    }
    else {
        __setFileContentHelper(parsedPath, nrfs.getRoot().folders, content);
    }
}

core::fileData core::pseudoFSBase::getFileData(std::string path) {
    return searchFileHelper(pathParser(path));
}

const core::NRFS& core::pseudoFSBase::getNRFS() {
    return nrfs;
}