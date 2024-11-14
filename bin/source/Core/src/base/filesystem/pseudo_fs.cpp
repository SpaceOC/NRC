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
#include "Core/base/utils.h"
#include "Core/base/print.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/users/permissions_enum_class.h"
//#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <chrono>

bool core::PseudoFS::isFile(std::string path) {
    if (path.find(".") >= LONG_LONG_MAX) return false;
    std::string file = path.substr(path.find("."), path.length());
    return (file.find(".") == 0);
}

int core::PseudoFS::__createFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const FolderData& oneFolderData) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;

    std::string currentFolderName = path.at(0);
    path.erase(path.begin());

    for (FolderData& folder : currentFolder.folders) {
        if (folder.name == currentFolderName)
            return __createFolderHelper(path, folder, oneFolderData);
    }

    if (path.size() < 2) {
        bool folderExists = std::any_of(currentFolder.folders.begin(), currentFolder.folders.end(), [&](const FolderData& f) { return f.name == oneFolderData.name; });
        
        if (folderExists) return core::PseudoFSCodes::ALREADY_EXISTS;
        FolderData newFolder = oneFolderData.name.empty() ? FolderData{currentFolderName, tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), {}, {}, false, false, nullptr, ""} : oneFolderData;
        currentFolder.folders.push_back(newFolder);
        nrfs.getRoot().update();
        return core::PseudoFSCodes::OK;
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__setFolderAttHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& what, const std::any& newAtt) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                if (what == "hidden") folder.hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") folder.system = std::any_cast<bool>(newAtt);
                folder.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                return core::PseudoFSCodes::OK;
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());
                return __setFolderAttHelper(path, folder, what, newAtt);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__renameFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& newName) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        bool newFolderExists = std::any_of(currentFolder.folders.begin(), currentFolder.folders.end(), [&](const FolderData& f) { return f.name == newName; });
        if (!newFolderExists) {
            for (FolderData& folder : currentFolder.folders) {
                if (folder.name == path.at(0)) {
                    folder.name = newName;
                    folder.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    return core::PseudoFSCodes::OK;
                }
            }
        }
        else
            return core::PseudoFSCodes::ALREADY_EXISTS;
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __renameFolderHelper(path, folder, newName);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__deleteFolderHelper(std::vector<std::string> path, FolderData& currentFolder) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (auto it = currentFolder.folders.begin(); it != currentFolder.folders.end(); ) {
            if (it->name == path.at(0)) {
                it = currentFolder.folders.erase(it);
                return core::PseudoFSCodes::OK;
            }
            else
                it++;
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());
                return __deleteFolderHelper(path, folder);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__moveFolderHelper(std::vector<std::string> path, FolderData& currentFolder, const core::FolderData& oldFolderData, const std::string& oldPath) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                bool exists = std::any_of(folder.folders.begin(), folder.folders.end(), [&](const FolderData& f) { return f.name == oldFolderData.name; });
                if (!exists) {
                    folder.folders.push_back(oldFolderData);
                    nrfs.getRoot().update();
                    return core::PseudoFSCodes::OK;
                }
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());
                return __moveFolderHelper(path, folder, oldFolderData, oldPath);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

core::FolderData core::PseudoFS::__getFolderData(std::vector<std::string> path, FolderData& currentFolder, int& code) {
    if (path.empty()) {
        code = core::PseudoFSCodes::PATH_IS_EMPTY;
        return {};
    }
    else if (path.size() == 1) {
        for (FolderData& anotherFolder : currentFolder.folders) {
            if (anotherFolder.name == path.at(0)) {
                if (anotherFolder.link != nullptr && !anotherFolder.linkPath.empty())
                    return getFolderData(anotherFolder.linkPath, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return anotherFolder;
                }
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __getFolderData(path, folder, code);
            }
        }
    }
    code = core::PseudoFSCodes::NOT_FOUND;
    return {};
}

int core::PseudoFS::__createFileHelper(std::vector<std::string> path, FolderData& currentFolder, const FileData& oneFileData) {
    if (path.size() == 1 && isFile(path.at(0))) {
        if (oneFileData.name.empty()) {
            currentFolder.files.push_back({path.at(0), "", tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), false, false, nullptr, ""});
        } else {
            currentFolder.files.push_back(oneFileData);
        }
        nrfs.getRoot().update();
        return core::PseudoFSCodes::OK;
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());

                if (!path.empty()) {
                    return __createFileHelper(path, folder, oneFileData);
                }
                else {
                    if (oneFileData.name.empty()) {
                        folder.files.push_back({folder.name, "", tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), false, false, nullptr, ""});
                    }
                    else
                        folder.files.push_back(oneFileData);
                }
                nrfs.getRoot().update();
                return core::PseudoFSCodes::OK;
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__renameFileHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& newName) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FileData& file : currentFolder.files) {
            bool newFileExists = std::any_of(currentFolder.files.begin(), currentFolder.files.end(), [&](const FileData& f) { return f.name == newName; });
            if (file.name == path.at(0) && !newFileExists) {
                file.name = newName;
                file.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                return core::PseudoFSCodes::OK;
            }
            else if (newFileExists) {
                return core::PseudoFSCodes::ALREADY_EXISTS;
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __renameFileHelper(path, folder, newName);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__deleteFileHelper(std::vector<std::string> path, FolderData& currentFolder) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1 && isFile(path.at(0))) {
        auto it = currentFolder.files.begin();
        for (FileData& file : currentFolder.files) {
            if (file.name == path.at(0)) {
                currentFolder.files.erase(it);
                return core::PseudoFSCodes::OK;
            }
            it++;
        }
        nrfs.getRoot().update();
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0) && !isFile(path.at(0))) {
                path.erase(path.begin());
                return __deleteFileHelper(path, folder);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__moveFileHelper(std::vector<std::string> path, FolderData& currentFolder, const core::FileData& oldFileData, const std::string& oldPath) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                bool exists = std::any_of(currentFolder.files.begin(), currentFolder.files.end(), [&](const FileData& f) { return f.name == oldFileData.name; });
                if (!exists) {
                    folder.files.push_back(oldFileData);
                    deleteFile(oldPath);
                    nrfs.getRoot().update();
                    return core::PseudoFSCodes::OK;
                }
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());
                return __moveFileHelper(path, folder, oldFileData, oldPath);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__setFileAttHelper(std::vector<std::string> path, FolderData& currentFolder, const std::string& what, const std::any& newAtt) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FileData& file : currentFolder.files) {
            if (file.name == path.at(0)) {
                if (what == "content") file.content = static_cast<std::string>(std::any_cast<const char*>(newAtt));
                else if (what == "hidden") file.hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") file.system = std::any_cast<bool>(newAtt);
                file.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                return core::PseudoFSCodes::OK;
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());
                return __setFileAttHelper(path, folder, what, newAtt);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

core::FileData core::PseudoFS::__getFileData(std::vector<std::string> path, FolderData& currentFolder, int& code) {
    if (path.empty()) {
        code = core::PseudoFSCodes::PATH_IS_EMPTY;
        return {};
    }
    else if (path.size() == 1) {
        for (FileData& file : currentFolder.files) {
            if (file.name == path.at(0)) {
                if (file.link != nullptr && !file.linkPath.empty())
                    return getFileData(file.linkPath, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return file;
                }
            }
        }
    }
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0)) {
                path.erase(path.begin());
                return __getFileData(path, folder, code);
            }
        }
    }
    code = core::PseudoFSCodes::NOT_FOUND;
    return {};
}

int core::PseudoFS::createFolder(std::string path, const FolderData& oneFolderData) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(),
            [&](const FolderData& f) { return f.name == parsedPath.at(1);}
        );
        if (folderExists)
            return core::PseudoFSCodes::ALREADY_EXISTS;

        if (oneFolderData.name.empty()) {
            nrfs.getRoot().folders.push_back({parsedPath.at(1), tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), {}, {}, false, false, nullptr, ""});
        }
        else
            nrfs.getRoot().folders.push_back(oneFolderData);
        nrfs.getRoot().update();
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __createFolderHelper(parsedPath, folder, oneFolderData);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::setFolderAtt(std::string path, std::string what, std::any newAtt) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(),
            [&](const FolderData& f) { return f.name == parsedPath.at(1); }
        );
        if (!folderExists)
            return core::PseudoFSCodes::NOT_FOUND;

        for (FolderData& folder : nrfs.getRoot().folders) {
            if (folder.name == parsedPath.at(1)) {
                if (what == "hidden") folder.hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") folder.system = std::any_cast<bool>(newAtt);
                folder.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __setFolderAttHelper(parsedPath, folder, what, newAtt);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::renameFolder(std::string path, std::string newName) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(),
            [&](const FolderData& f) { return f.name == parsedPath.at(1); }
        );
        if (!folderExists)
            return core::PseudoFSCodes::NOT_FOUND;

        bool newFolderExists = std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(),
            [&](const FolderData& f) { return f.name == newName; }
        );
        if (newFolderExists)
                return core::PseudoFSCodes::ALREADY_EXISTS;
        
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (folder.name == parsedPath.at(1)) {
                folder.name = newName;
                folder.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __renameFolderHelper(parsedPath, folder, newName);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::deleteFolder(std::string path) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(), [&](const FolderData& f) { return f.name == parsedPath.at(1); });
        if (!folderExists)
            return core::PseudoFSCodes::NOT_FOUND;

        auto it = nrfs.getRoot().folders.begin();
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (folder.name == parsedPath.at(1)) {
                nrfs.getRoot().folders.erase(it);
                nrfs.getRoot().update();
                break;
            }
            it++;
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __deleteFolderHelper(parsedPath, folder);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::moveFolder(std::string path, const std::string& newPath) {
    std::vector<std::string> parsedPath = core::Utils::split(newPath, '/');
    int code = 0;
    const FolderData oldFolderData = getFolderData(path, code);
    if (code != 1)
        return code;
    if (parsedPath.size() == 1) {
        bool newFolderExists = std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(), [&](const FolderData& f) { return f.name == oldFolderData.name; });
        if (newFolderExists)
            return core::PseudoFSCodes::ALREADY_EXISTS;

        nrfs.getRoot().folders.push_back(oldFolderData);
        deleteFolder(path);
        nrfs.getRoot().update();
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                if (parsedPath.size() > 2) parsedPath.erase(parsedPath.begin());
                int code = __moveFolderHelper(parsedPath, folder, oldFolderData, path);
                if (code)
                    deleteFolder(path);
                return code;
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

core::FolderData core::PseudoFS::getFolderData(std::string path, int& code) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (folder.name == parsedPath.at(1)) {
                if (folder.link != nullptr && !folder.linkPath.empty())
                    return getFolderData(folder.linkPath, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return folder;
                }
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());

        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __getFolderData(parsedPath, folder, code);
            }
        }
    }
    code = core::PseudoFSCodes::NOT_FOUND;
    return {};
}

int core::PseudoFS::createFile(std::string path, const FileData& oneFileData) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');

    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs.getRoot().files.begin(), nrfs.getRoot().files.end(),
            [&](const FileData& f) { return f.name == parsedPath.at(1); }
        );
        if (fileExists)
            return core::PseudoFSCodes::ALREADY_EXISTS;

        if (oneFileData.name.empty()) {
            nrfs.getRoot().files.push_back({parsedPath.at(1), "", tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), false, false, nullptr, ""});
        }
        else
            nrfs.getRoot().files.push_back(oneFileData);
        nrfs.getRoot().update();
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __createFileHelper(parsedPath, folder, oneFileData);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::renameFile(std::string path, std::string newName) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs.getRoot().files.begin(), nrfs.getRoot().files.end(),
            [&](const FileData& f) { return f.name == parsedPath.at(1); }
        );
        if (!fileExists)
            return core::PseudoFSCodes::NOT_FOUND;

        bool newFileExists = std::any_of(nrfs.getRoot().files.begin(), nrfs.getRoot().files.end(),
            [&](const FileData& f) { return f.name == newName; }
        );
        if (newFileExists)
                return core::PseudoFSCodes::ALREADY_EXISTS;

        for (FileData& file : nrfs.getRoot().files) {
            if (file.name == parsedPath.at(1)) {
                file.name = newName;
                file.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __renameFileHelper(parsedPath, folder, newName);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::deleteFile(std::string path) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs.getRoot().files.begin(), nrfs.getRoot().files.end(),
            [&](const FileData& f) { return f.name == parsedPath.at(1); }
        );
        if (!fileExists)
            return core::PseudoFSCodes::NOT_FOUND;    

        auto it = nrfs.getRoot().files.begin();
        for (FileData& file : nrfs.getRoot().files) {
            if (file.name == parsedPath.at(1))
                nrfs.getRoot().files.erase(it);
            it++;
        }
        nrfs.getRoot().update();
    }
    else {
        parsedPath.erase(parsedPath.begin()); 
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __deleteFileHelper(parsedPath, folder);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::moveFile(std::string path, const std::string& newPath) {
    try {
        std::vector<std::string> parsedPath = core::Utils::split(newPath, '/');
        int code = 0;
        const FileData& oldFileData = getFileData(path, code);
        if (code != 1)
            return code;
        if (parsedPath.size() == 1) {
            bool newFileExists = std::any_of(nrfs.getRoot().files.begin(), nrfs.getRoot().files.end(),
                [&](const FileData& f) { return f.name == oldFileData.name; }
            );
            if (newFileExists)
                return core::PseudoFSCodes::ALREADY_EXISTS;

            nrfs.getRoot().files.push_back(oldFileData);
            deleteFile(path);
            nrfs.getRoot().update();
        }
        else {
            parsedPath.erase(parsedPath.begin());

            for (FolderData& folder : nrfs.getRoot().folders) {
                if (parsedPath.at(0) == folder.name) {
                    if (parsedPath.size() > 2) parsedPath.erase(parsedPath.begin());
                    return __moveFileHelper(parsedPath, folder, oldFileData, path);
                }
            }
            return core::PseudoFSCodes::NOT_FOUND;
        }
    }
    catch(const std::exception& e) {
        core::print("Error: " + static_cast<std::string>(e.what()) + "\n", core::PrintColors::red);
        return core::PseudoFSCodes::UNKNOWN_ERROR;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::setFileAtt(std::string path, std::string what, std::any newAtt) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs.getRoot().files.begin(), nrfs.getRoot().files.end(),
            [&](const FileData& f) { return f.name == parsedPath.at(1); }
        );
        if (!fileExists)
            return core::PseudoFSCodes::NOT_FOUND;

        for (FileData& file : nrfs.getRoot().files) {
            if (file.name == parsedPath.at(1)) {
                if (what == "content") file.content = static_cast<std::string>(std::any_cast<const char*>(newAtt));
                else if (what == "hidden") file.hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") file.system = std::any_cast<bool>(newAtt);
                file.dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs.getRoot().update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __setFileAttHelper(parsedPath, folder, what, newAtt);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

core::FileData core::PseudoFS::getFileData(std::string path, int& code) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        for (FileData& file : nrfs.getRoot().files) {
            if (file.name == parsedPath.at(1)) {
                if (file.link != nullptr && !file.linkPath.empty())
                    return getFileData(file.linkPath, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return file;
                }
            }
        }
        code = core::PseudoFSCodes::NOT_FOUND;
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (parsedPath.at(0) == folder.name) {
                parsedPath.erase(parsedPath.begin());
                return __getFileData(parsedPath, folder, code);
            }
        }
        code = core::PseudoFSCodes::NOT_FOUND;
    }
    return {};
}

void core::PseudoFS::printAllHelper(const std::vector<FolderData> &folders, const std::string &path, bool includeHidden) {
    for (const FolderData &folder : folders) {
        std::string newPath = path + folder.name + "/";

        if (!folder.folders.empty()) {
            bool showFolder = includeHidden && folder.hidden;
            if (!showFolder) showFolder = !folder.hidden;
            
            if (showFolder)
                printAllHelper(folder.folders, newPath, includeHidden);
        }

        if (!folder.files.empty()) {
            for (const FileData &file : folder.files) {
                bool showFile = includeHidden && file.hidden;
                if (!showFile) showFile = !file.hidden;

                if (showFile)
                    std::cout << newPath << file.name << "\n";
            }
        }

        if (folder.folders.empty() && folder.files.empty()) {
            bool showFolder = includeHidden && folder.hidden;
            if (!showFolder) showFolder = !folder.hidden;

            if (showFolder)
                std::cout << newPath << '\n';
        }
    }
}

void core::PseudoFS::printAll(bool includeHidden, std::string startPath) {
    std::vector<std::string> parsedPath = core::Utils::split(startPath, '/');
    if (parsedPath.size() < 2) {
        parsedPath = {};
        for (const FolderData &folder : nrfs.getRoot().folders) {
            std::string newPath = "./" + folder.name + "/";
            
            if (!folder.folders.empty()) {
                bool showFolder = includeHidden && folder.hidden;
                if (!showFolder) showFolder = !folder.hidden;
                    printAllHelper(folder.folders, newPath, includeHidden);
            }

            if (!folder.files.empty()) {
                for (const FileData &file : folder.files) {
                    bool showFile = includeHidden && file.hidden;
                    if (!showFile) showFile = !file.hidden;

                    if (showFile)
                        std::cout << newPath << file.name << "\n";
                }
            }

            if (folder.folders.empty() && folder.files.empty()) {
                bool showFolder = includeHidden && folder.hidden;
                if (!showFolder) showFolder = !folder.hidden;

                if (showFolder)
                    std::cout << newPath << '\n';
            }
        }

        for (const FileData &file : nrfs.getRoot().files) {
            bool showFile = includeHidden && file.hidden;
            if (!showFile) showFile = !file.hidden;

            if (showFile)
                std::cout << "./" << file.name << '\n';
        }
    }
    else {
        parsedPath = {};
        int code = 0;
        printAllHelper(getFolderData(startPath, code).folders, startPath, includeHidden);
    }
}

void core::PseudoFS::showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level) {
    for (const FolderData &folder : curFolder.folders) {
        bool showFolder = includeHidden && folder.hidden;
        if (!showFolder) showFolder = !folder.hidden;

        if (folder.folders.empty() && showFolder)
            std::cout << std::string(level * 2, ' ') << folder.name << '\n';
        else if (!folder.folders.empty() && showFolder) {
            std::cout << std::string(level * 2, ' ') << folder.name << '\n';
            showTreeHelper(folder, includeHidden, showFiles, level + 1);
        }

        if (showFiles && !folder.files.empty()) {
            for (const FileData &file : folder.files) {
               bool showFile = includeHidden && file.hidden;
                if (!showFile) showFile = !file.hidden;

                if (showFile)
                    std::cout << std::string((level + 1) * 2, ' ') << file.name << '\n';
            }
        }
    }
    if (!showFiles) return;
    for (const FileData &file : curFolder.files) {
        bool showFile = includeHidden && file.hidden;
        if (!showFile) showFile = !file.hidden;

        if (showFile)
            std::cout << std::string(level * 2, ' ') << file.name << '\n';
    }
}

void core::PseudoFS::showTree(bool includeHidden, bool showFiles, std::string startPath) {
    std::vector<std::string> parsedPath = core::Utils::split(startPath, '/');
    if (parsedPath.size() < 2) {
        parsedPath = {};
        int level = 0;
        for (const FolderData &folder : nrfs.getRoot().folders) {
            bool showFolder = includeHidden && folder.hidden;
            if (!showFolder) showFolder = !folder.hidden;

            if (folder.folders.empty() && showFolder)
                std::cout << folder.name << '\n';
            else if (!folder.folders.empty() && showFolder) {
                std::cout << folder.name << '\n';
                showTreeHelper(folder, includeHidden, showFiles, level + 1);
            }
        }
        if (!showFiles) return;
        for (const FileData &file : nrfs.getRoot().files) {
            bool showFile = includeHidden && file.hidden;
            if (!showFile) showFile = !file.hidden;

            if (showFile)
                std::cout << file.name << '\n';
        }
    }
    else {
        parsedPath = {};
        int code = 0;
        showTreeHelper(getFolderData(startPath, code), includeHidden, showFiles, 0);
    }
}

void core::PseudoFS::printDiskSize() {
    std::cout << nrfs.getRoot().getDiskSize() << '\n';
}

core::NRFS& core::PseudoFS::getNRFS() {
    return nrfs;
}

void core::PseudoFS::init() {
    int code = 0;
    getFolderData("./home", code);
    if (code == core::PseudoFSCodes::NOT_FOUND) {
        createFolder("./home");
        setFolderAtt("./home", "system", true);
    }
}

nlohmann::json core::PseudoFS::__savePFSHelper(std::vector<core::FolderData>& folders) {
    nlohmann::json jsonTemp;
    for (FolderData& folder : folders) {
        jsonTemp.push_back({"folder", folder.name, folder.id, folder.dataCreate, folder.dataEdit, __savePFSHelper(folder.files) ,__savePFSHelper(folder.folders), folder.system, folder.hidden});
    }
    if (jsonTemp.empty())
        jsonTemp = "null";
    return jsonTemp;
}

nlohmann::json core::PseudoFS::__savePFSHelper(std::vector<core::FileData>& files) {
    nlohmann::json jsonTemp;
    for (FileData& file : files)
        jsonTemp.push_back({"file", file.name, file.content, file.id, file.dataCreate, file.dataEdit, file.system, file.hidden});
    if (jsonTemp.empty())
        jsonTemp = "null";
    return jsonTemp;
}

void core::PseudoFS::savePFS() {
    nlohmann::json PFSJsonData;
    for (FolderData& folder : nrfs.getRoot().folders) {
        PFSJsonData["."].push_back({"folder", folder.name, folder.id, folder.dataCreate, folder.dataEdit, __savePFSHelper(folder.files) ,__savePFSHelper(folder.folders), folder.system, folder.hidden, folder.link == nullptr, folder.linkPath});
    }
    for (FileData& file : nrfs.getRoot().files) {
        PFSJsonData["."].push_back({"file", file.name, file.content, file.id, file.dataCreate, file.dataEdit, file.system, file.hidden, file.link == nullptr, file.linkPath});
    }
	std::ofstream file("Data/PFS-Data.json", std::ios::out);
    file << PFSJsonData.dump(4) << '\n';
	file.close();
}

std::vector<core::FolderData> core::PseudoFS::__loadPFSHelperFolders(nlohmann::json folderJsonData) {
    std::vector<core::FolderData> temp = {};
    for (const auto &a : folderJsonData) {
        if (a.is_array()) {
            int id = a.at(2).get<int>();
            bool system = a.at(7).get<bool>();
            bool hidden = a.at(8).get<bool>();
            tempID = (id > tempID ? id : tempID);
            FolderData folder = {a.at(1), a.at(2), a.at(3), a.at(4), __loadPFSHelperFiles(a.at(5)), __loadPFSHelperFolders(a.at(6)), system, hidden, nullptr, ""};
            temp.push_back(folder);
        }
    }
    return temp;
}

std::vector<core::FileData> core::PseudoFS::__loadPFSHelperFiles(nlohmann::json filesJsonData) {
    std::vector<core::FileData> temp = {};
    for (const auto &a : filesJsonData) {
        if (a.is_array()) {
            int id = a.at(3).get<int>();
            bool system = a.at(6).get<bool>();
            bool hidden = a.at(7).get<bool>();
            tempID = (id > tempID ? id : tempID);
            FileData file = {a.at(1), a.at(2), a.at(3), a.at(4), a.at(5), system, hidden, nullptr, ""};
            temp.push_back(file);
        }
    }
    return temp;
}

void core::PseudoFS::__loadLinksFoldersAndFiles(FolderData& curFolder) {
    for (FolderData& folder : curFolder.folders) {
        if (folder.linkPath.empty()) continue;
        int code;
        FolderData linkFolder = getFolderData(folder.linkPath, code);
        folder.link = &linkFolder;
        if (!folder.folders.empty() || !folder.files.empty()) __loadLinksFoldersAndFiles(folder);
    }
    for (FileData& file : curFolder.files) {
        if (file.linkPath.empty()) continue;
        int code;
        FileData linkFile = getFileData(file.linkPath, code);
        file.link = &linkFile;
    }
}

int core::PseudoFS::loadPFS() {
    FileManager FM;
    if (!FM.fileExist("Data/PFS-Data.json")) {
        FM.createFile("Data/PFS-Data.json");
        core::print("Error: PFS-Data.json not found!\n", core::PrintColors::red);
        init();
        return core::PseudoFSCodes::NOT_FOUND;
    }
    else if (FM.readFile("Data/PFS-Data.json").empty()) {
        core::print("Error: PFS-Data.json is empty!\n", core::PrintColors::red);
        init();
        return core::PseudoFSCodes::IS_EMPTY;
    }
    nlohmann::json jsonData = nlohmann::json::parse(FM.readFile("Data/PFS-Data.json"));
    try {
        for (nlohmann::json jsonDataPart : jsonData["."]) {
            if (jsonDataPart.at(0).get<std::string>() == "file") {
                int id = jsonDataPart.at(3).get<int>();
                bool system = jsonDataPart.at(6).get<bool>();
                bool hidden = jsonDataPart.at(7).get<bool>();
                tempID = (id > tempID ? id : tempID);
                createFile("./a", {jsonDataPart.at(1), jsonDataPart.at(2), jsonDataPart.at(3), jsonDataPart.at(4), jsonDataPart.at(5), system, hidden, nullptr, jsonDataPart.at(9)});
            }
            else {
                int id = jsonDataPart.at(2).get<int>();
                bool system = jsonDataPart.at(7).get<bool>();
                bool hidden = jsonDataPart.at(8).get<bool>();
                tempID = (id > tempID ? id : tempID);
                std::vector<FolderData> folders = __loadPFSHelperFolders((jsonDataPart.at(6) != "null" ? jsonDataPart.at(6) : ""));
                std::vector<FileData> files = __loadPFSHelperFiles((jsonDataPart.at(5) != "null" ? jsonDataPart.at(5) : ""));
                createFolder("./a", {jsonDataPart.at(1), id, jsonDataPart.at(3), jsonDataPart.at(4), files, folders, system, hidden, nullptr, jsonDataPart.at(10)});
            }
        }
        for (FolderData& folder : nrfs.getRoot().folders) {
            if (folder.linkPath.empty()) continue;
            int code;
            FolderData linkFolder = getFolderData(folder.linkPath, code);
            folder.link = &linkFolder;
            if (!folder.folders.empty() || !folder.files.empty()) __loadLinksFoldersAndFiles(folder);
        }
        for (FileData& file : nrfs.getRoot().files) {
            if (file.linkPath.empty()) continue;
            int code;
            FileData linkFile = getFileData(file.linkPath, code);
            file.link = &linkFile;
        }
        ++tempID;
    }
    catch(const std::exception& e) {
        core::print("Error: " + static_cast<std::string>(e.what()) + "\n", core::PrintColors::red);
        return core::PseudoFSCodes::JSON_PARSING_FAILED;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::changePath(const std::string& newPath) {
    if (folderExists(newPath) || newPath == "./") {
        currentPath = newPath;
        return core::PseudoFSCodes::OK;
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::changeDirectory(const std::string& newDirectory) {
    std::vector<std::string> parsedPath = core::Utils::split(currentPath, '/');
    if (newDirectory == ".." && parsedPath.size() > 1) {
        std::string currentPathTemp;
        for (const std::string& pathPart : parsedPath)
            currentPathTemp += pathPart + "/";
        currentPath = currentPathTemp.substr(0, currentPathTemp.length() - (parsedPath.back().length() + 1));
        return core::PseudoFSCodes::OK;
    }
    else if (newDirectory == ".." && parsedPath.size() == 1)
        return core::PseudoFSCodes::SMALL_SIZE;
    else {
        if (folderExists(currentPath + newDirectory)) {
            currentPath += newDirectory + "/";
            return core::PseudoFSCodes::OK;
        }
    }
    return core::PseudoFSCodes::BAD_PATH;
}

bool core::PseudoFS::__folderExistsHelper(std::vector<std::string> path, FolderData& currentFolder) {
    if (currentFolder.folders.empty() || path.empty())
        return false;
    else {
        for (FolderData& folder : currentFolder.folders) {
            if (folder.name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __folderExistsHelper(path, folder);
            }
            if (path.size() == 1 && path.at(0) == folder.name) {
                return true;
            }
        }
    }
    return false;
}

bool core::PseudoFS::folderExists(const std::string& path) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    try {
        if (parsedPath.size() < 2)
            return false;
        if (parsedPath.size() == 2) {
            return std::any_of(nrfs.getRoot().folders.begin(), nrfs.getRoot().folders.end(), [&](const FolderData& f) { return f.name == parsedPath.at(1); });
        }
        else {
            parsedPath.erase(parsedPath.begin());
            for (FolderData& folder : nrfs.getRoot().folders) {
                if (parsedPath.at(0) == folder.name) {
                    parsedPath.erase(parsedPath.begin());
                    return __folderExistsHelper(parsedPath, folder);
                }
            }
        }
    }
    catch(const std::exception& e) {
        core::print("Error: " + static_cast<std::string>(e.what()) + "\n", core::PrintColors::red);
    }
    return false;
}

const std::string core::PseudoFS::getCurrentPath() {
    return currentPath;
}