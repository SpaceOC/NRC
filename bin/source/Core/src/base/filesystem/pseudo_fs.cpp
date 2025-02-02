#include "Core/base/filesystem/pseudo_fs.h"
#include "Core/base/filesystem/nrfs.h"
#include "Core/base/utils.h"
#include "Core/base/print.h"
#include "Core/logs.h"
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

std::string core::pseudoFSCodesS(int t) {
    switch(t) {
        case PseudoFSCodes::OK:
            return "Ok";
            break;
        case PseudoFSCodes::NOT_FOUND:
            return "Not found";
            break;
        case PseudoFSCodes::ALREADY_EXISTS:
            return "Already exists";
            break;
        case PseudoFSCodes::PATH_IS_EMPTY:
            return "Path is empty";
            break;
        case PseudoFSCodes::THIS_LINK:
            return "This is link";
            break;
        case PseudoFSCodes::BAD_PATH:
            return "Bad path";
            break;
        case PseudoFSCodes::JSON_PARSING_FAILED:
            return "JSON parsing failed";
            break;
        case PseudoFSCodes::IS_EMPTY:
            return "Is empty";
            break;
        case PseudoFSCodes::SMALL_SIZE:
            return "Small size";
            break;
    }
    return "Unknown error";
}

core::PseudoFS PFS;
core::PseudoFS* core::pseudoFS() {
    return &PFS;
}

bool core::PseudoFS::isFile(std::string path) {
    if (path.find(".") >= LONG_LONG_MAX) return false;
    std::string file = path.substr(path.find("."), path.length());
    return (file.find(".") == 0);
}

int core::PseudoFS::__createFolderHelper(std::vector<std::string> path, FolderData* currentFolder, FolderData* oneFolderData) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;

    std::string currentFolderName = path.at(0);
    path.erase(path.begin());

    for (FolderData* folder : currentFolder->folders) {
        if (folder->name == currentFolderName)
            return __createFolderHelper(path, folder, oneFolderData);
    }

    if (path.size() < 2) {
        bool folderExists = std::any_of(currentFolder->folders.begin(), currentFolder->folders.end(), [&](FolderData* f) {
            if (!oneFolderData) return false;
            return f->name == oneFolderData->name;
        });
        
        if (folderExists) return core::PseudoFSCodes::ALREADY_EXISTS;
        FolderData newFolder = !oneFolderData ? FolderData{currentFolderName, tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), {}, {}, false, false, nullptr, ""} : *oneFolderData;
        currentFolder->folders.push_back(new FolderData(newFolder));
        //nrfs->disks[curDisk]->update();
        return core::PseudoFSCodes::OK;
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__setFolderAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                if (what == "hidden") folder->hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") folder->system = std::any_cast<bool>(newAtt);
                else if (what == "owner") folder->owner = std::any_cast<User*>(newAtt);
                folder->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                //nrfs->disks[curDisk]->update();
                return core::PseudoFSCodes::OK;
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                return __setFolderAttHelper(path, folder, what, newAtt);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__renameFolderHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        bool newFolderExists = std::any_of(currentFolder->folders.begin(), currentFolder->folders.end(), [&](FolderData* f) { return f->name == newName; });
        if (!newFolderExists) {
            for (FolderData* folder : currentFolder->folders) {
                if (folder->name == path.at(0)) {
                    folder->name = newName;
                    folder->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    return core::PseudoFSCodes::OK;
                }
            }
        }
        else
            return core::PseudoFSCodes::ALREADY_EXISTS;
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __renameFolderHelper(path, folder, newName);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__deleteFolderHelper(std::vector<std::string> path, FolderData* currentFolder) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (auto it = currentFolder->folders.begin(); it != currentFolder->folders.end(); ) {
            if ((*it)->name == path.at(0)) {
                it = currentFolder->folders.erase(it);
                return core::PseudoFSCodes::OK;
            }
            else
                it++;
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                return __deleteFolderHelper(path, folder);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__moveFolderHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const core::FolderData& oldFolderData, const std::string& oldPath) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                bool exists = std::any_of(folder->folders.begin(), folder->folders.end(), [&](FolderData* f) { return f->name == oldFolderData.name; });
                if (!exists) {
                    folder->folders.push_back(new FolderData(oldFolderData));
                    nrfs->disks[diskId]->update();
                    return core::PseudoFSCodes::OK;
                }
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                return __moveFolderHelper(path, diskId, folder, oldFolderData, oldPath);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

core::FolderData core::PseudoFS::__getFolderData(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code) {
    if (path.empty()) {
        code = core::PseudoFSCodes::PATH_IS_EMPTY;
        return {};
    }
    else if (path.size() == 1) {
        for (FolderData* anotherFolder : currentFolder->folders) {
            if (anotherFolder->name == path.at(0)) {
                if (anotherFolder->link != nullptr && !anotherFolder->linkPath.empty())
                    return getFolderData(anotherFolder->linkPath, diskId, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return *anotherFolder;
                }
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __getFolderData(path, diskId, folder, code);
            }
        }
    }
    code = core::PseudoFSCodes::NOT_FOUND;
    return {};
}

int core::PseudoFS::__createFileHelper(std::vector<std::string> path, FolderData* currentFolder, FileData* oneFileData) {
    if (path.size() == 1 && isFile(path.at(0))) {
        if (!oneFileData) {
            currentFolder->files.push_back(new FileData(
                {
                    path.at(0),
                    "",
                    tempID++,
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                    false,
                    false,
                    nullptr,
                    ""
                }
            ));
        } else {
            currentFolder->files.push_back(new FileData(*oneFileData));
        }
        //nrfs->disks[curDisk]->update();
        return core::PseudoFSCodes::OK;
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                /*
                if (!path.empty()) {
                    return __createFileHelper(path, folder, oneFileData);
                }
                else {
                    if (oneFileData.name.empty()) {
                        folderfiles.push_back({folder->name, "", tempID++, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), false, false, nullptr, ""});
                    }
                    else
                        folder->files.push_back(new FileData(oneFileData));
                }
                nrfs->disks[curDisk]->update();
                return core::PseudoFSCodes::OK;
                */
                return __createFileHelper(path, folder, oneFileData);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__renameFileHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& newName) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FileData* file : currentFolder->files) {
            bool newFileExists = std::any_of(currentFolder->files.begin(), currentFolder->files.end(), [&](FileData* f) { return f->name == newName; });
            if (file->name == path.at(0) && !newFileExists) {
                file->name = newName;
                file->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                //nrfs->disks[curDisk]->update();
                return core::PseudoFSCodes::OK;
            }
            else if (newFileExists) {
                return core::PseudoFSCodes::ALREADY_EXISTS;
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __renameFileHelper(path, folder, newName);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__deleteFileHelper(std::vector<std::string> path, FolderData* currentFolder) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1 && isFile(path.at(0))) {
        auto it = currentFolder->files.begin();
        for (FileData* file : currentFolder->files) {
            if (file->name == path.at(0)) {
                delete file;
                currentFolder->files.erase(it);
                return core::PseudoFSCodes::OK;
            }
            it++;
        }
        //nrfs->disks[curDisk]->update();
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0) && !isFile(path.at(0))) {
                path.erase(path.begin());
                return __deleteFileHelper(path, folder);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__moveFileHelper(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, const core::FileData& oldFileData, const std::string& oldPath) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                bool exists = std::any_of(currentFolder->files.begin(), currentFolder->files.end(), [&](FileData* f) { return f->name == oldFileData.name; });
                if (!exists) {
                    folder->files.push_back(new FileData(oldFileData));
                    deleteFile(oldPath, diskId);
                    //nrfs->disks[curDisk]->update();
                    return core::PseudoFSCodes::OK;
                }
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                return __moveFileHelper(path, diskId, folder, oldFileData, oldPath);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::__setFileAttHelper(std::vector<std::string> path, FolderData* currentFolder, const std::string& what, const std::any& newAtt) {
    if (path.empty())
        return core::PseudoFSCodes::PATH_IS_EMPTY;
    else if (path.size() == 1) {
        for (FileData* file : currentFolder->files) {
            if (file->name == path.at(0)) {
                if (what == "content") file->content = newAtt.type() != typeid(std::string) ?
                                static_cast<std::string>(std::any_cast<const char*>(newAtt)) :
                                std::any_cast<std::string>(newAtt);
                else if (what == "hidden") file->hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") file->system = std::any_cast<bool>(newAtt);
                else if (what == "owner") file->owner = std::any_cast<User*>(newAtt);
                file->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                //nrfs->disks[curDisk]->update();
                return core::PseudoFSCodes::OK;
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                return __setFileAttHelper(path, folder, what, newAtt);
            }
        }
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

core::FileData core::PseudoFS::__getFileData(std::vector<std::string> path, size_t diskId, FolderData* currentFolder, int& code) {
    if (path.empty()) {
        code = core::PseudoFSCodes::PATH_IS_EMPTY;
        return {};
    }
    else if (path.size() == 1) {
        for (FileData* file : currentFolder->files) {
            if (file->name == path.at(0)) {
                if (file->link != nullptr && !file->linkPath.empty())
                    return getFileData(file->linkPath, diskId, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return *file;
                }
            }
        }
    }
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0)) {
                path.erase(path.begin());
                return __getFileData(path, diskId, folder, code);
            }
        }
    }
    code = core::PseudoFSCodes::NOT_FOUND;
    return {};
}

int core::PseudoFS::createFolder(std::string path, size_t diskId, FolderData* oneFolderData) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs->disks[diskId]->folders.begin(), nrfs->disks[diskId]->folders.end(),
            [&](FolderData* f) { return f->name == parsedPath.at(1);}
        );
        if (folderExists)
            return core::PseudoFSCodes::ALREADY_EXISTS;

        if (!oneFolderData) {
            nrfs->disks[diskId]->folders.push_back(new FolderData(
                {
                    parsedPath.at(1),
                    tempID++,
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                    {},
                    {},
                    false,
                    false,
                    nullptr,
                    ""
                }
            ));
        }
        else
            nrfs->disks[diskId]->folders.push_back(new FolderData(*oneFolderData));
        nrfs->disks[diskId]->update();
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                int code = __createFolderHelper(parsedPath, folder, oneFolderData);
                nrfs->disks[diskId]->update();
                return code;
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::setFolderAtt(std::string path, size_t diskId, std::string what, std::any newAtt) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs->disks[diskId]->folders.begin(), nrfs->disks[diskId]->folders.end(),
            [&](FolderData* f) { return f->name == parsedPath.at(1); }
        );
        if (!folderExists)
            return core::PseudoFSCodes::NOT_FOUND;

        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (folder->name == parsedPath.at(1)) {
                if (what == "hidden") folder->hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") folder->system = std::any_cast<bool>(newAtt);
                else if (what == "owner") folder->owner = std::any_cast<User*>(newAtt);
                folder->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs->disks[diskId]->update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                int code = __setFolderAttHelper(parsedPath, folder, what, newAtt);
                nrfs->disks[diskId]->update();
                return code;
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::renameFolder(std::string path, size_t diskId, std::string newName) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs->disks[diskId]->folders.begin(), nrfs->disks[diskId]->folders.end(),
            [&](FolderData* f) { return f->name == parsedPath.at(1); }
        );
        if (!folderExists)
            return core::PseudoFSCodes::NOT_FOUND;

        bool newFolderExists = std::any_of(nrfs->disks[diskId]->folders.begin(), nrfs->disks[diskId]->folders.end(),
            [&](FolderData* f) { return f->name == newName; }
        );
        if (newFolderExists)
                return core::PseudoFSCodes::ALREADY_EXISTS;
        
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (folder->name == parsedPath.at(1)) {
                folder->name = newName;
                folder->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs->disks[diskId]->update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                int code = __renameFolderHelper(parsedPath, folder, newName);
                nrfs->disks[diskId]->update();
                return code;
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::deleteFolder(std::string path, size_t diskId) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool folderExists = std::any_of(nrfs->disks[diskId]->folders.begin(), nrfs->disks[diskId]->folders.end(), [&](FolderData* f) { return f->name == parsedPath.at(1); });
        if (!folderExists)
            return core::PseudoFSCodes::NOT_FOUND;

        auto it = nrfs->disks[diskId]->folders.begin();
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (folder->name == parsedPath.at(1)) {
                nrfs->disks[diskId]->folders.erase(it);
                nrfs->disks[diskId]->update();
                break;
            }
            it++;
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                int code = __deleteFolderHelper(parsedPath, folder);
                nrfs->disks[diskId]->update();
                return code;
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::moveFolder(std::string path, size_t diskId, const std::string& newPath, size_t anotherDiskId) {
    std::vector<std::string> parsedPath = core::Utils::split(newPath, '/');
    int code = 0;
    const FolderData oldFolderData = getFolderData(path, diskId, code);
    if (code != 1)
        return code;
    if (parsedPath.size() == 1) {
        bool newFolderExists = std::any_of(nrfs->disks[anotherDiskId]->folders.begin(), nrfs->disks[anotherDiskId]->folders.end(), [&](FolderData* f) { return f->name == oldFolderData.name; });
        if (newFolderExists)
            return core::PseudoFSCodes::ALREADY_EXISTS;

        nrfs->disks[anotherDiskId]->folders.push_back(new FolderData(oldFolderData));
        deleteFolder(path, diskId);
        nrfs->disks[anotherDiskId]->update();
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[anotherDiskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                if (parsedPath.size() > 2) parsedPath.erase(parsedPath.begin());
                int code = __moveFolderHelper(parsedPath, anotherDiskId, folder, oldFolderData, path);
                if (code) {
                    deleteFolder(path, diskId);
                    nrfs->disks[diskId]->update();
                }
                return code;
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

core::FolderData core::PseudoFS::getFolderData(std::string path, size_t diskId, int& code) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (folder->name == parsedPath.at(1)) {
                if (folder->link != nullptr && !folder->linkPath.empty())
                    return getFolderData(folder->linkPath, diskId, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return *folder;
                }
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());

        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                return __getFolderData(parsedPath, diskId, folder, code);
            }
        }
    }
    code = core::PseudoFSCodes::NOT_FOUND;
    return {};
}

int core::PseudoFS::createFile(std::string path, size_t diskId, FileData* oneFileData) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');

    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs->disks[diskId]->files.begin(), nrfs->disks[diskId]->files.end(),
            [&](FileData* f) { return f->name == parsedPath.at(1); }
        );
        if (fileExists)
            return core::PseudoFSCodes::ALREADY_EXISTS;

        if (!oneFileData) {
            nrfs->disks[diskId]->files.push_back(new FileData(
                {
                    parsedPath.at(1),
                    "",
                    tempID++,
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                    false,
                    false,
                    nullptr,
                    ""
                }
            ));
        }
        else
            nrfs->disks[diskId]->files.push_back(new FileData(*oneFileData));
        nrfs->disks[diskId]->update();
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                return __createFileHelper(parsedPath, folder, oneFileData);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::renameFile(std::string path, size_t diskId, std::string newName) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs->disks[diskId]->files.begin(), nrfs->disks[diskId]->files.end(),
            [&](FileData* f) { return f->name == parsedPath.at(1); }
        );
        if (!fileExists)
            return core::PseudoFSCodes::NOT_FOUND;

        bool newFileExists = std::any_of(nrfs->disks[diskId]->files.begin(), nrfs->disks[diskId]->files.end(),
            [&](FileData* f) { return f->name == newName; }
        );
        if (newFileExists)
                return core::PseudoFSCodes::ALREADY_EXISTS;

        for (FileData* file : nrfs->disks[diskId]->files) {
            if (file->name == parsedPath.at(1)) {
                file->name = newName;
                file->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs->disks[diskId]->update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                return __renameFileHelper(parsedPath, folder, newName);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::deleteFile(std::string path, size_t diskId) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs->disks[diskId]->files.begin(), nrfs->disks[diskId]->files.end(),
            [&](FileData* f) { return f->name == parsedPath.at(1); }
        );
        if (!fileExists)
            return core::PseudoFSCodes::NOT_FOUND;    

        auto it = nrfs->disks[diskId]->files.begin();
        for (FileData* file : nrfs->disks[diskId]->files) {
            if (file->name == parsedPath.at(1)) {
                delete file;
                nrfs->disks[diskId]->files.erase(it);
            }
            it++;
        }
        nrfs->disks[diskId]->update();
    }
    else {
        parsedPath.erase(parsedPath.begin()); 
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                return __deleteFileHelper(parsedPath, folder);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::moveFile(std::string path, size_t diskId, const std::string& newPath, size_t anotherDiskId) {
    try {
        std::vector<std::string> parsedPath = core::Utils::split(newPath, '/');
        int code = 0;
        const FileData& oldFileData = getFileData(path, diskId, code);
        if (code != 1)
            return code;
        if (parsedPath.size() == 1) {
            bool newFileExists = std::any_of(nrfs->disks[anotherDiskId]->files.begin(), nrfs->disks[anotherDiskId]->files.end(),
                [&](FileData* f) { return f->name == oldFileData.name; }
            );
            if (newFileExists)
                return core::PseudoFSCodes::ALREADY_EXISTS;

            nrfs->disks[anotherDiskId]->files.push_back(new FileData(oldFileData));
            deleteFile(path, diskId);
            nrfs->disks[anotherDiskId]->update();
        }
        else {
            parsedPath.erase(parsedPath.begin());

            for (FolderData* folder : nrfs->disks[diskId]->folders) {
                if (parsedPath.at(0) == folder->name) {
                    if (parsedPath.size() > 2) parsedPath.erase(parsedPath.begin());
                    return __moveFileHelper(parsedPath, diskId, folder, oldFileData, path);
                }
            }
            return core::PseudoFSCodes::NOT_FOUND;
        }
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "moveFile Error: ", e.what(), "\n"
            "target file: ", getFileFromPath(path), "\n",
            "path: " , path, "\n"
            "new path: ", newPath
        );
        #ifdef NRC_WEB
		outputLog += "moveFile Error: " + std::string(e.what()) + "\n" +
            "target file: " + getFileFromPath(path) + "\n" +
            "path: " + path + "\n" +
            "new path: " + newPath + "\n";
		#endif
        return core::PseudoFSCodes::UNKNOWN_ERROR;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::setFileAtt(std::string path, size_t diskId, std::string what, std::any newAtt) {
    assert(!(newAtt.type() != typeid(std::string) && newAtt.type() != typeid(const char*) && newAtt.type() != typeid(bool) && newAtt.type() != typeid(User)));
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        bool fileExists = std::any_of(nrfs->disks[diskId]->files.begin(), nrfs->disks[diskId]->files.end(),
            [&](FileData* f) { return f->name == parsedPath.at(1); }
        );
        if (!fileExists)
            return core::PseudoFSCodes::NOT_FOUND;

        for (FileData* file : nrfs->disks[diskId]->files) {
            if (file->name == parsedPath.at(1)) {
                if (what == "content") file->content = newAtt.type() != typeid(std::string) ?
                                static_cast<std::string>(std::any_cast<const char*>(newAtt)) :
                                std::any_cast<std::string>(newAtt);
                else if (what == "hidden") file->hidden = std::any_cast<bool>(newAtt);
                else if (what == "system") file->system = std::any_cast<bool>(newAtt);
                else if (what == "owner") file->owner = std::any_cast<User*>(newAtt);
                file->dataEdit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                nrfs->disks[diskId]->update();
                break;
            }
        }
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                return __setFileAttHelper(parsedPath, folder, what, newAtt);
            }
        }
        return core::PseudoFSCodes::NOT_FOUND;
    }
    return core::PseudoFSCodes::OK;
}

core::FileData core::PseudoFS::getFileData(std::string path, size_t diskId, int& code) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    if (parsedPath.size() == 2) {
        for (FileData* file : nrfs->disks[diskId]->files) {
            if (file->name == parsedPath.at(1)) {
                if (file->link != nullptr && !file->linkPath.empty())
                    return getFileData(file->linkPath, diskId, code);
                else {
                    code = core::PseudoFSCodes::OK;
                    return *file;
                }
            }
        }
        code = core::PseudoFSCodes::NOT_FOUND;
    }
    else {
        parsedPath.erase(parsedPath.begin());
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            if (parsedPath.at(0) == folder->name) {
                parsedPath.erase(parsedPath.begin());
                return __getFileData(parsedPath, diskId, folder, code);
            }
        }
        code = core::PseudoFSCodes::NOT_FOUND;
    }
    return {};
}

void core::PseudoFS::printAllHelper(const std::vector<FolderData*> &folders, const std::string &path, bool includeHidden) {
    for (FolderData* folder : folders) {
        std::string newPath = path + folder->name + "/";

        if (!folder->folders.empty()) {
            bool showFolder = includeHidden && folder->hidden;
            if (!showFolder) showFolder = !folder->hidden;
            
            if (showFolder) {
                printAllHelper(folder->folders, newPath, includeHidden);
            }
        }

        if (!folder->files.empty()) {
            for (FileData* file : folder->files) {
                bool showFile = includeHidden && file->hidden;
                if (!showFile) showFile = !file->hidden;

                if (showFile) {
                    std::cout << newPath << file->name << "\n";
                }
            }
        }

        if (folder->folders.empty() && folder->files.empty()) {
            bool showFolder = includeHidden && folder->hidden;
            if (!showFolder) showFolder = !folder->hidden;

            if (showFolder) {
                std::cout << newPath << '\n';
            }
        }
    }
}

void core::PseudoFS::printAllHelper(const std::vector<FolderData*> &folders, const std::string &path, bool includeHidden, std::string& str) {
    std::string output;
    for (FolderData* folder : folders) {
        std::string newPath = path + folder->name + "/";

        if (!folder->folders.empty()) {
            bool showFolder = includeHidden && folder->hidden;
            if (!showFolder) showFolder = !folder->hidden;
            
            if (showFolder) {
                std::string temp;
                printAllHelper(folder->folders, newPath, includeHidden, temp);
                output += temp + "\n";
            }
        }

        if (!folder->files.empty()) {
            for (FileData* file : folder->files) {
                bool showFile = includeHidden && file->hidden;
                if (!showFile) showFile = !file->hidden;

                if (showFile) {
                    output += newPath + file->name + "\n";
                }
            }
        }

        if (folder->folders.empty() && folder->files.empty()) {
            bool showFolder = includeHidden && folder->hidden;
            if (!showFolder) showFolder = !folder->hidden;

            if (showFolder) {
                output += newPath + "\n";
            }
        }
    }
    str = output;
}

void core::PseudoFS::printAll(bool includeHidden, size_t diskId, std::string startPath) {
    std::vector<std::string> parsedPath = core::Utils::split(startPath, '/');
    if (parsedPath.size() < 2) {
        parsedPath = {};
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            std::string newPath = "./" + folder->name + "/";
            
            if (!folder->folders.empty()) {
                bool showFolder = includeHidden && folder->hidden;
                if (!showFolder) showFolder = !folder->hidden;
                if (showFolder) {
                    printAllHelper(folder->folders, newPath, includeHidden);
                }
            }

            if (!folder->files.empty()) {
                for (FileData* file : folder->files) {
                    bool showFile = includeHidden && file->hidden;
                    if (!showFile) showFile = !file->hidden;

                    if (showFile) {
                        std::cout << newPath << file->name << "\n";
                    }
                }
            }

            if (folder->folders.empty() && folder->files.empty()) {
                bool showFolder = includeHidden && folder->hidden;
                if (!showFolder) showFolder = !folder->hidden;

                if (showFolder) {
                    std::cout << newPath << '\n';
                }
            }
        }

        for (FileData* file : nrfs->disks[diskId]->files) {
            bool showFile = includeHidden && file->hidden;
            if (!showFile) showFile = !file->hidden;

            if (showFile) {
                std::cout << "./" << file->name << '\n';
            }
        }
    }
    else {
        parsedPath = {};
        int code = 0;
        printAllHelper(getFolderData(startPath, diskId, code).folders, startPath, includeHidden);
    }
}

void core::PseudoFS::printAll(bool includeHidden, size_t diskId, std::string startPath, std::string& str) {
    std::string result;
    std::vector<std::string> parsedPath = core::Utils::split(startPath, '/');
    if (parsedPath.size() < 2) {
        parsedPath = {};
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            std::string newPath = "./" + folder->name + "/";
            
            if (!folder->folders.empty()) {
                bool showFolder = includeHidden && folder->hidden;
                if (!showFolder) showFolder = !folder->hidden;
                if (showFolder) {
                    std::string temp;
                    printAllHelper(folder->folders, newPath, includeHidden, temp);
                    result += temp + "\n";
                }
            }

            if (!folder->files.empty()) {
                for (FileData* file : folder->files) {
                    bool showFile = includeHidden && file->hidden;
                    if (!showFile) showFile = !file->hidden;

                    if (showFile) {
                        result += newPath + file->name + "\n";
                    }
                }
            }

            if (folder->folders.empty() && folder->files.empty()) {
                bool showFolder = includeHidden && folder->hidden;
                if (!showFolder) showFolder = !folder->hidden;

                if (showFolder) {
                    result += newPath + "\n";
                }
            }
        }

        for (FileData* file : nrfs->disks[diskId]->files) {
            bool showFile = includeHidden && file->hidden;
            if (!showFile) showFile = !file->hidden;

            if (showFile) {
                result += "./" + file->name + "\n";
            }
        }
    }
    else {
        parsedPath = {};
        int code = 0;
        std::string temp;
        printAllHelper(getFolderData(startPath, diskId, code).folders, startPath, includeHidden, temp);
        result += temp;
    }

    str = result;
}

void core::PseudoFS::showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level) {
    std::string output;
    for (FolderData* folder : curFolder.folders) {
        bool showFolder = includeHidden && folder->hidden;
        if (!showFolder) showFolder = !folder->hidden;

        if (folder->folders.empty() && showFolder) {
            std::cout << std::string(level * 2, ' ') << folder->name << '\n';
        }
        else if (!folder->folders.empty() && showFolder) {
            std::cout << std::string(level * 2, ' ') << folder->name << '\n';
            showTreeHelper(*folder, includeHidden, showFiles, level + 1);
        }

        if (showFiles && !folder->files.empty()) {
            for (FileData* file : folder->files) {
               bool showFile = includeHidden && file->hidden;
                if (!showFile) showFile = !file->hidden;

                if (showFile) {
                    std::cout << std::string((level + 1) * 2, ' ') << file->name << '\n';
                }
            }
        }
    }
    if (showFiles) {
        for (FileData* file : curFolder.files) {
            bool showFile = includeHidden && file->hidden;
            if (!showFile) showFile = !file->hidden;

            if (showFile) {
                std::cout << std::string(level * 2, ' ') << file->name << '\n';
            }
        }
    }
}

void core::PseudoFS::showTreeHelper(const FolderData &curFolder, bool includeHidden, bool showFiles, int level, std::string& str) {
    std::string output;
    for (FolderData* folder : curFolder.folders) {
        bool showFolder = includeHidden && folder->hidden;
        if (!showFolder) showFolder = !folder->hidden;

        if (folder->folders.empty() && showFolder) {
            output += std::string(level * 2, ' ') + folder->name + "\n";
        }
        else if (!folder->folders.empty() && showFolder) {
            output += std::string(level * 2, ' ') + folder->name + "\n";
            std::string temp;
            showTreeHelper(*folder, includeHidden, showFiles, level + 1, temp);
            output += temp;
        }

        if (showFiles && !folder->files.empty()) {
            for (FileData* file : folder->files) {
               bool showFile = includeHidden && file->hidden;
                if (!showFile) showFile = !file->hidden;

                if (showFile) {
                    output += std::string((level + 1) * 2, ' ') + file->name + "\n";
                }
            }
        }
    }
    if (showFiles) {
        for (FileData* file : curFolder.files) {
            bool showFile = includeHidden && file->hidden;
            if (!showFile) showFile = !file->hidden;

            if (showFile) {
                output += std::string(level * 2, ' ') + file->name + "\n";
            }
        }
    }
    str = output;
}

void core::PseudoFS::showTree(bool includeHidden, size_t diskId, bool showFiles, std::string startPath) {
    std::vector<std::string> parsedPath = core::Utils::split(startPath, '/');
    if (parsedPath.size() < 2) {
        parsedPath = {};
        int level = 0;
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            bool showFolder = includeHidden && folder->hidden;
            if (!showFolder) showFolder = !folder->hidden;

            if (folder->folders.empty() && showFolder) {
                std::cout << folder->name << '\n';
            }
            else if (!folder->folders.empty() && showFolder) {
                std::cout << folder->name << '\n';
                showTreeHelper(*folder, includeHidden, showFiles, level + 1);
            }
        }
        if (showFiles) {
            for (FileData* file : nrfs->disks[diskId]->files) {
                bool showFile = includeHidden && file->hidden;
                if (!showFile) showFile = !file->hidden;

                if (showFile) {
                    std::cout << file->name << '\n';
                }
            }
        }
    }
    else {
        parsedPath = {};
        int code = 0;
        showTreeHelper(getFolderData(startPath, diskId, code), includeHidden, showFiles, 0);
    }
}

void core::PseudoFS::showTree(bool includeHidden, size_t diskId, bool showFiles, std::string startPath, std::string& str) {
    std::string result;
    std::vector<std::string> parsedPath = core::Utils::split(startPath, '/');
    if (parsedPath.size() < 2) {
        parsedPath = {};
        int level = 0;
        for (FolderData* folder : nrfs->disks[diskId]->folders) {
            bool showFolder = includeHidden && folder->hidden;
            if (!showFolder) showFolder = !folder->hidden;

            if (folder->folders.empty() && showFolder) {
                result += folder->name + "\n";
            }
            else if (!folder->folders.empty() && showFolder) {
                result += folder->name + "\n";
                std::string temp;
                showTreeHelper(*folder, includeHidden, showFiles, level + 1, temp);
                result += temp;
            }
        }
        if (showFiles) {
            for (FileData* file : nrfs->disks[diskId]->files) {
                bool showFile = includeHidden && file->hidden;
                if (!showFile) showFile = !file->hidden;

                if (showFile) {
                    result += file->name + "\n";
                }
            }
        }
    }
    else {
        parsedPath = {};
        int code = 0;
        std::string temp;
        showTreeHelper(getFolderData(startPath, diskId, code), includeHidden, showFiles, 0, temp);
        result += temp + "\n";
    }
    str = result;
}

core::NRFS* core::PseudoFS::getNRFS() {
    return nrfs;
}

void core::PseudoFS::init() {
   nrfs = new NRFS();
}

void core::PseudoFS::postInit() {
    try {
        int code = 0;
        getFolderData("./home", 0, code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            createFolder("./home", 0);
            setFolderAtt("./home", 0, "system", true);
            createFile("./hello.txt", 0);
            setFileAtt("./hello.txt", 0, "content", "Hello from NRC!");
            savePFS();
        }
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "PFS Post Init Error: ", e.what()
        );
        #ifdef NRC_WEB
		outputLog += "PFS Post Init Error: " + std::string(e.what()) + "\n";
		#endif
    }
}

nlohmann::json core::PseudoFS::__savePFSHelper(std::vector<core::FolderData*>& folders) {
    nlohmann::json jsonTemp;
    try {
        for (FolderData* folder : folders) {
            if (folder == nullptr) continue;
            // по каким-то причинам один и тот же объект сохраняется два раза. Этот топорный фикс призван решить данную проблему :/
            else if (!jsonTemp.empty() && folder->name == jsonTemp.back().at(1).get<std::string>()) continue;
            jsonTemp.push_back(
                {
                    "folder",
                    folder->name,
                    folder->id,
                    folder->dataCreate,
                    folder->dataEdit,
                    __savePFSHelper(folder->files),
                    __savePFSHelper(folder->folders),
                    folder->system,
                    folder->hidden,
                    folder->link == nullptr,
                    folder->linkPath,
                    (folder->owner ? folder->owner->getUsername() : "")
                }
            );
        }
        if (jsonTemp.empty())
            jsonTemp = {};
        return jsonTemp;
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "SavePFS error: ", e.what(), "\n",
            "json data: ", jsonTemp
        );
    	#ifdef NRC_WEB
		outputLog += "SavePFS Error: " + std::string(e.what()) + "\n" +
            "json data: " + core::Utils::valueToString(jsonTemp) + "\n";
		#endif
    }
    return {};
}

nlohmann::json core::PseudoFS::__savePFSHelper(std::vector<core::FileData*>& files) {
    nlohmann::json jsonTemp;
    try {
        for (FileData* file : files) {
            if (file == nullptr) continue;
            // по каким-то причинам один и тот же объект сохраняется два раза. Этот топорный фикс призван решить данную проблему :/
            else if (!jsonTemp.empty() && file->name == jsonTemp.back().at(1).get<std::string>()) continue;
            jsonTemp.push_back(
                {
                    "file",
                    file->name,
                    file->content,
                    file->id,
                    file->dataCreate,
                    file->dataEdit,
                    file->system,
                    file->hidden,
                    file->link == nullptr,
                    file->linkPath,
                    (file->owner ? file->owner->getUsername() : "")
                }
            );
        }
        if (jsonTemp.empty())
            jsonTemp = {};
        return jsonTemp;
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "SavePFS error: ", e.what(), "\n",
            "json data: ", jsonTemp
        );
    	#ifdef NRC_WEB
		outputLog += "SavePFS Error: " + std::string(e.what()) + "\n" +
            "json data: " + core::Utils::valueToString(jsonTemp) + "\n";
		#endif
    }
    return {};
}

void core::PseudoFS::savePFS() {
    nlohmann::json PFSJsonData;
    try {
        for (size_t diskId = 0; diskId < nrfs->disks.size(); diskId++) {
            nlohmann::json diskJsonData;

            std::string letterStr;
            letterStr.push_back(nrfs->disks[diskId]->letter);

            diskJsonData.push_back({
                "metadata",
                letterStr,
                nrfs->disks[diskId]->name
            });

            for (FolderData* folder : nrfs->disks[diskId]->folders) {
                if (folder == nullptr) continue;
                // По каким-то причинам один и тот же объект сохраняется два раза. По идее эта строчка должна решить данную проблему :/
                else if (!diskJsonData.empty() && diskJsonData.back().at(0).get<std::string>() != "metadata" && folder->name == diskJsonData.back().at(1).get<std::string>()) continue;
                diskJsonData.push_back(
                    {
                        "folder",
                        folder->name,
                        folder->id,
                        folder->dataCreate,
                        folder->dataEdit,
                        __savePFSHelper(folder->files),
                        __savePFSHelper(folder->folders),
                        folder->system,
                        folder->hidden,
                        folder->link == nullptr,
                        folder->linkPath,
                        (folder->owner ? folder->owner->getUsername() : "")
                    }
                );
            }

            for (FileData* file : nrfs->disks[diskId]->files) {
                if (file == nullptr) continue;
                // По каким-то причинам один и тот же объект сохраняется два раза. По идее эта строчка должна решить данную проблему :/
                else if (!diskJsonData.empty() && diskJsonData.back().at(0).get<std::string>() != "metadata" && file->name == diskJsonData.back().at(1).get<std::string>()) continue;
                diskJsonData.push_back(
                    {
                        "file",
                        file->name,
                        file->content,
                        file->id,
                        file->dataCreate,
                        file->dataEdit,
                        file->system,
                        file->hidden,
                        file->link == nullptr,
                        file->linkPath,
                        (file->owner ? file->owner->getUsername() : "")
                    }
                );
            }
            PFSJsonData.push_back(diskJsonData);
        }
        std::ofstream file("Data/PFS-Data.json", std::ios::out);
        file << PFSJsonData.dump(1) << '\n';
        file.close();
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "SavePFS error: ", e.what(), "\n",
            "json data: ", PFSJsonData
        );
        #ifdef NRC_WEB
		outputLog += "SavePFS Error: " + std::string(e.what()) + "\n" +
            "json data: " + core::Utils::valueToString(PFSJsonData) + "\n";
		#endif
    }
}

std::vector<core::FolderData*> core::PseudoFS::__loadPFSHelperFolders(nlohmann::json folderJsonData) {
    std::vector<core::FolderData*> temp = {};
    for (const auto &a : folderJsonData) {
        if (a.is_array()) {
            int id = a.at(2).get<int>();
            bool system = a.at(7).get<bool>();
            bool hidden = a.at(8).get<bool>();

            std::string username = a.at(11).get<std::string>();
            User* folderOwner = nullptr;
            if (!username.empty() && core::userManager()->userExist(username))
                folderOwner = &core::userManager()->getUser(username);

            tempID = (id > tempID ? id : tempID);

            FolderData folder = {
                a.at(1),
                a.at(2),
                a.at(3),
                a.at(4),
                __loadPFSHelperFiles(a.at(5)),
                __loadPFSHelperFolders(a.at(6)),
                system,
                hidden,
                nullptr,
                a.at(10).get<std::string>(),
                (folderOwner ? folderOwner : nullptr)
            };

            temp.push_back(new FolderData(folder));
        }
    }
    return temp;
}

std::vector<core::FileData*> core::PseudoFS::__loadPFSHelperFiles(nlohmann::json filesJsonData) {
    std::vector<core::FileData*> temp = {};
    for (const auto &a : filesJsonData) {
        if (a.is_array()) {
            int id = a.at(3).get<int>();
            bool system = a.at(6).get<bool>();
            bool hidden = a.at(7).get<bool>();

            std::string username = a.at(10).get<std::string>();
            User* fileOwner = nullptr;
            if (!username.empty() && core::userManager()->userExist(username))
                fileOwner = &core::userManager()->getUser(username);

            tempID = (id > tempID ? id : tempID);

            FileData file = {
                a.at(1),
                a.at(2),
                a.at(3),
                a.at(4),
                a.at(5),
                system,
                hidden,
                nullptr,
                a.at(9).get<std::string>(),
                (fileOwner ? fileOwner : nullptr)
            };
            
            temp.push_back(new FileData(file));
        }
    }
    return temp;
}

void core::PseudoFS::__loadLinksFoldersAndFiles(FolderData* curFolder, size_t diskId) {
    for (FolderData* folder : curFolder->folders) {
        if (folder->linkPath.empty()) continue;
        int code;
        FolderData linkFolder = getFolderData(folder->linkPath, diskId,  code);
        folder->link = &linkFolder;
        if (!folder->folders.empty() || !folder->files.empty()) __loadLinksFoldersAndFiles(folder, diskId);
    }
    for (FileData* file : curFolder->files) {
        if (file->linkPath.empty()) continue;
        int code;
        FileData linkFile = getFileData(file->linkPath, diskId, code);
        file->link = &linkFile;
    }
}

int core::PseudoFS::loadPFS() {
    FileManager FM;
    if (!FM.fileExist("Data/PFS-Data.json")) {
        FM.createFile("Data/PFS-Data.json");
        trace(core::PrintColors::red, "Error: PFS-Data.json not found!");
        init();
        return core::PseudoFSCodes::NOT_FOUND;
    }
    else if (FM.readFile("Data/PFS-Data.json").empty() || FM.readFile("Data/PFS-Data.json")._Starts_with("null")) {
        trace(core::PrintColors::red, "Error: PFS-Data.json is empty!");
        init();
        return core::PseudoFSCodes::IS_EMPTY;
    }
    nlohmann::json jsonData = nlohmann::json::parse(FM.readFile("Data/PFS-Data.json"));
    try {
        for (nlohmann::json diskJsonData : jsonData) {
            for (nlohmann::json jsonDataPart : diskJsonData) {
                if (jsonDataPart.at(0).get<std::string>() == "metadata") {
                    char c = jsonDataPart.at(1).get<std::string>().at(0);
                    std::string name = jsonDataPart.at(2).get<std::string>();
                    nrfs->createDisk(c);
                    nrfs->renameDisk(c, name);
                    curDisk = 0;
                    for (auto d : nrfs->disks) {
                        if (d->letter == c)
                            break;
                        ++curDisk;
                    }
                }
                else if (jsonDataPart.at(0).get<std::string>() == "file") {
                    int id = jsonDataPart.at(3).get<int>();
                    bool system = jsonDataPart.at(6).get<bool>();
                    bool hidden = jsonDataPart.at(7).get<bool>();

                    std::string username = jsonDataPart.at(10).get<std::string>();
                    User* fileOwner = nullptr;
                    if (!username.empty() && core::userManager()->userExist(username))
                        fileOwner = &core::userManager()->getUser(username);

                    tempID = (id > tempID ? id : tempID);

                    FileData* file = new FileData{
                        jsonDataPart.at(1),
                        jsonDataPart.at(2),
                        jsonDataPart.at(3),
                        jsonDataPart.at(4),
                        jsonDataPart.at(5),
                        system,
                        hidden,
                        nullptr,
                        jsonDataPart.at(9),
                        (fileOwner ? fileOwner : nullptr)
                    };

                    createFile("./a", curDisk, file);
                }
                else {
                    int id = jsonDataPart.at(2).get<int>();
                    bool system = jsonDataPart.at(7).get<bool>();
                    bool hidden = jsonDataPart.at(8).get<bool>();

                    std::string username = jsonDataPart.at(11).get<std::string>();
                    User* folderOwner = nullptr;
                    if (!username.empty() && core::userManager()->userExist(username))
                        folderOwner = &core::userManager()->getUser(username);

                    tempID = (id > tempID ? id : tempID);
                    std::vector<FolderData*> folders = __loadPFSHelperFolders((!jsonDataPart.at(6).empty() ? jsonDataPart.at(6) : ""));
                    std::vector<FileData*> files = __loadPFSHelperFiles((!jsonDataPart.at(5).empty() ? jsonDataPart.at(5) : ""));
                    
                    FolderData* folder = new FolderData{
                        jsonDataPart.at(1),
                        id,
                        jsonDataPart.at(3),
                        jsonDataPart.at(4),
                        files,
                        folders,
                        system,
                        hidden,
                        nullptr,
                        jsonDataPart.at(10),
                        (folderOwner ? folderOwner : nullptr)
                    };
                    
                    createFolder("./a", curDisk, folder);
                }
            }
            curDisk = 0;
            for (size_t diskId = 0; diskId < nrfs->disks.size(); diskId++) {
                for (FolderData* folder : nrfs->disks[diskId]->folders) {
                    if (folder->linkPath.empty()) continue;
                    int code;
                    FolderData linkFolder = getFolderData(folder->linkPath, diskId, code);
                    folder->link = &linkFolder;
                    if (!folder->folders.empty() || !folder->files.empty()) __loadLinksFoldersAndFiles(folder, diskId);
                }
                for (FileData* file : nrfs->disks[diskId]->files) {
                    if (file->linkPath.empty()) continue;
                    int code;
                    FileData linkFile = getFileData(file->linkPath, diskId, code);
                    file->link = &linkFile;
                }
            }
            ++tempID;
            curDisk = 0;
        }
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "NRFS JSON PARSING ERROR: ", e.what(), "\n",
            "json data: ", jsonData
        );
        #ifdef NRC_WEB
		outputLog += "NRFS JSON PARSING ERROR: " + std::string(e.what()) + "\n" +
            "json data: " + core::Utils::valueToString(jsonData) + "\n";
		#endif
        return core::PseudoFSCodes::JSON_PARSING_FAILED;
    }
    return core::PseudoFSCodes::OK;
}

int core::PseudoFS::changePath(const std::string& newPath, size_t diskId) {
    if (folderExists(newPath, diskId) || newPath == "./") {
        currentPath = newPath;
        return core::PseudoFSCodes::OK;
    }
    return core::PseudoFSCodes::NOT_FOUND;
}

int core::PseudoFS::changeDirectory(const std::string& newDirectory, size_t diskId) {
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
        if (folderExists(currentPath + newDirectory, diskId)) {
            currentPath += newDirectory + "/";
            return core::PseudoFSCodes::OK;
        }
    }
    return core::PseudoFSCodes::BAD_PATH;
}

int core::PseudoFS::changeDisk(char diskLetter) {
    size_t tempDiskId = 0;
    for (auto d : nrfs->disks) {
        if (d->letter == diskLetter) {
            curDisk = tempDiskId;
            currentPath = "./"; // Для избежания возможных ошибок при смене текущего диска.
            break;
        }
        ++tempDiskId;
    }
    return core::PseudoFSCodes::OK;
}

bool core::PseudoFS::__folderExistsHelper(std::vector<std::string> path, FolderData* currentFolder) {
    if (currentFolder->folders.empty() || path.empty())
        return false;
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __folderExistsHelper(path, folder);
            }
            if (path.size() == 1 && path.at(0) == folder->name) {
                return true;
            }
        }
    }
    return false;
}

bool core::PseudoFS::folderExists(const std::string& path, size_t diskId) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    try {
        if (parsedPath.size() < 2)
            return false;
        if (parsedPath.size() == 2) {
            return std::any_of(nrfs->disks[diskId]->folders.begin(), nrfs->disks[diskId]->folders.end(), [&](FolderData* f) { return f->name == parsedPath.at(1); });
        }
        else {
            parsedPath.erase(parsedPath.begin());
            for (FolderData* folder : nrfs->disks[diskId]->folders) {
                if (parsedPath.at(0) == folder->name) {
                    parsedPath.erase(parsedPath.begin());
                    return __folderExistsHelper(parsedPath, folder);
                }
            }
        }
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "folderExists Error: ", e.what(), "\n",
            "folder: ", getFolderFromPath(path),
            "path: ", path
        );
		#ifdef NRC_WEB
		outputLog += "folderExists Error: " + std::string(e.what()) + "\n" +
            "folder: " + getFolderFromPath(path) + "\n" +
            "path: " + path + "\n";
		#endif
    }
    return false;
}

bool core::PseudoFS::__fileExistsHelper(std::vector<std::string> path, FolderData* currentFolder) {
    if (currentFolder->folders.empty() || path.empty())
        return false;
    else {
        for (FolderData* folder : currentFolder->folders) {
            if (folder->name == path.at(0) && path.size() > 1) {
                path.erase(path.begin());
                return __fileExistsHelper(path, folder);
            }
            else if (path.size() == 1) {
                return std::any_of(currentFolder->files.begin(), currentFolder->files.end(), [&](FileData* f) { return f->name == path.at(0); });
            }
        }
    }
    return false;
}

bool core::PseudoFS::fileExists(const std::string& path, size_t diskId) {
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    try {
        if (parsedPath.size() < 2)
            return false;
        if (parsedPath.size() == 2) {
            return std::any_of(nrfs->disks[diskId]->files.begin(), nrfs->disks[diskId]->files.end(), [&](FileData* f) { return f->name == parsedPath.at(1); });
        }
        else {
            parsedPath.erase(parsedPath.begin());
            for (FolderData* folder : nrfs->disks[diskId]->folders) {
                if (parsedPath.at(0) == folder->name) {
                    parsedPath.erase(parsedPath.begin());
                    return __fileExistsHelper(parsedPath, folder);
                }
            }
        }
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "fileExists Error: ", e.what(), "\n",
            "folder: ", getFolderFromPath(path), "\n",
            "target file", getFileFromPath(path), "\n", 
            "path: ", path
        );
		#ifdef NRC_WEB
		outputLog += "folderExists Error: " + std::string(e.what()) + "\n" +
            "folder: " + getFolderFromPath(path) + "\n" +
            "path: " + path + "\n";
		#endif
    }
    return false;
}

const std::string core::PseudoFS::getCurrentPath() {
    return currentPath;
}

std::string core::PseudoFS::getFileFromPath(const std::string& path) {
    size_t index = path.find_last_of("/");
    if (index >= path.length() && path.find_last_of(".") < 1) return "";
    return path.substr(index + 1, path.length());
}

std::string core::PseudoFS::getFolderFromPath(const std::string& path) {
    size_t index = path.find_last_of("/");
    if (index >= path.length()) return "";
    return path.substr(index + 1, path.length());
}