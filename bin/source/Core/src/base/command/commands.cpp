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
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <chrono>
#include "Core/base/print.h"
#include "Core/base/command/commands.h"
#include "Core/base/command/handler_commands.h"
#include "Core/CORE_info.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/filesystem/pseudo_fs.h"
#include "Core/base/utils.h"

void core::commands::CORE_COMMAND_help(const std::vector<std::string>& args) {
    if (args.empty()) {
        for (const auto& command : core::HandlerCommands::getAllCommands()) {
            std::string argsNames = "";
            if (!command.second.argsNames.empty()) {
                for (auto arg : command.second.argsNames) {
                    argsNames += " <" + arg + ">";
                }
            }
            print(command.first + argsNames + command.second.description + '\n', PrintColors::light_green);
        }
    }
    else {
        if (core::HandlerCommands::getCommand(args.at(0)).empty())
            core::print("Command not found\n");
        else {
            std::map<std::string, core::CommandDescription> temp = core::HandlerCommands::getCommand(args.at(0));
            std::string argsNames = "";
            if (!temp[args.at(0)].argsNames.empty()) {
                for (auto arg : temp[args.at(0)].argsNames) {
                    argsNames += " <" + arg + ">";
                }
            }
            print(args.at(0) + argsNames + temp[args.at(0)].description + '\n', PrintColors::light_green);
        }
    }
};

void core::commands::CORE_COMMAND_info() {
    std::cout << CORE_NAME << " by " << CORE_DEVELOPER << '\n' << CORE_VERSION << " " << CORE_VERSION_TYPE << '\n';
    std::cout << "NRFS by SpaceOC" << '\n' << NRFS_VERSION << " " << NRFS_VERSION_TYPE << '\n';
    std::cout << "JSON library by nlohmann - https://github.com/nlohmann/json \n";
    std::cout << " --- Special Thanks ---\nAlone Knight - migrating NRC from Makefile to CMake\n";
};

void core::commands::CORE_COMMAND_time(const std::vector<std::string>& args) {
    auto start = std::chrono::steady_clock::now();
    std::string temp;

    for (const std::string& anotherTemp : args) {
        if (anotherTemp != args.at(0))
            temp += (anotherTemp == core::HandlerCommands::getCommandSeparator() ? "" : "\"") + anotherTemp + (anotherTemp != args.back() ? (anotherTemp == core::HandlerCommands::getCommandSeparator() ? "" : "\" ") : (anotherTemp == core::HandlerCommands::getCommandSeparator() ? "" : "\""));
        else
            temp += anotherTemp + " ";
    }
    core::CommandObject command = core::HandlerCommands::parsing(temp).at(0);
    core::HandlerCommands::sendCommand(command);
    auto end = std::chrono::steady_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    core::print("time: ", core::PrintColors::light_green);
    std::cout << duration;
    core::print("ms\n", core::PrintColors::light_green);
}

/*
void core::commands:: core::commands:: CORE_COMMAND_exit() {
    work = false;
}
*/

//   -------------- Pseudo FS Commands ---------------


void core::commands::CORE_COMMAND_cd(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0) == ".." && !args.at(0)._Starts_with("./") ? ".." : args.at(0));
        int code;
        FolderData folder = core::PseudoFS::getFolderData((where != ".." ? (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + where) : "./"), code);
        if ((where != ".." || args.at(0)._Starts_with("./"))) {
            if (folder.link != nullptr && !folder.linkPath.empty()) {
                if (!core::PseudoFS::changePath(folder.linkPath)) {
                    core::print("Error: Change path operation failed!\n", core::PrintColors::red);
                }
            }
            else {
                if (core::PseudoFS::changeDirectory(where)) {
                    core::print("Error: Change directory operation failed!\n", core::PrintColors::red);
                }
            }
        }
        else {
            if (!core::PseudoFS::changePath((folder.link != nullptr && !folder.linkPath.empty() ? folder.linkPath : where))) {
                core::print("Error: Change path operation failed!\n", core::PrintColors::red);
            }
        }
    }
}

void core::commands::CORE_COMMAND_createFile(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        switch (core::PseudoFS::createFile(where)) {
            case core::PseudoFSCodes::NOT_FOUND:
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This file already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("The file creation operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_createLinkFile(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        std::string where2 = (args.at(1)._Starts_with("./") ? args.at(1) : core::PseudoFS::getCurrentPath() + args.at(1));
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        int code;
        FileData file = core::PseudoFS::getFileData(where2, code);
        switch (core::PseudoFS::createFile(where, {core::Utils::split(where, '/').back(), "", -1, 0, 0, false, file.hidden, &file, where2})) {
            case core::PseudoFSCodes::NOT_FOUND:
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This file already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("The file creation operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_deleteFile(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        int code;
        core::PseudoFS::getFileData(where, code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("File not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        else if (core::UserManager::currentUserData().getPermissions() < Permissions::admin && core::PseudoFS::getFileData(where, code).system) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }

        core::PseudoFS::deleteFile(where);
        core::PseudoFS::savePFS();
    }
}

void core::commands::CORE_COMMAND_renameFile(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return;
    }
    else {
        int code;
        core::PseudoFS::getFileData(args.at(0), code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("File not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        else if (core::UserManager::currentUserData().getPermissions() < Permissions::admin && core::PseudoFS::getFileData(args.at(0), code).system) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }

        switch (core::PseudoFS::renameFile(args.at(0), args.at(1))) {
            case core::PseudoFSCodes::NOT_FOUND:
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This file already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("The file renaming operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_moveFile(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return;
    }
    else {
        int code;
        core::PseudoFS::getFileData(args.at(0), code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("File not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        else if (core::UserManager::currentUserData().getPermissions() < Permissions::admin && core::PseudoFS::getFileData(args.at(0), code).system) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }

        switch (core::PseudoFS::moveFile(args.at(0), args.at(1)))  {
            case core::PseudoFSCodes::NOT_FOUND:
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This file already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("File move operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_showFileData(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        int code;
        core::PseudoFS::getFileData(where, code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("File not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user && (core::PseudoFS::getFileData(where, code).hidden || core::PseudoFS::getFileData(where, code).system)) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        
        core::print("File name: " + core::PseudoFS::getFileData(where, code).name + "\n", core::PrintColors::aqua);
        core::print("File content: " + core::PseudoFS::getFileData(where, code).content + "\n", core::PrintColors::aqua);
        core::print("File ID: " + std::to_string(core::PseudoFS::getFileData(where, code).id) + "\n", core::PrintColors::aqua);
    }
}

void core::commands::CORE_COMMAND_createFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        switch (core::PseudoFS::createFolder(where)) {
            case core::PseudoFSCodes::NOT_FOUND: 
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This folder already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("Folder creation operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_createLinkFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return;
    }
    else {
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        std::string where2 = (args.at(1)._Starts_with("./") ? args.at(1) : core::PseudoFS::getCurrentPath() + args.at(1));
        int code;
        FolderData folder = core::PseudoFS::getFolderData(where2, code);
        switch (core::PseudoFS::createFolder(where, {core::Utils::split(where, '/').back(), -1, 0, 0, {}, {}, false, folder.hidden, &folder, where2})) {
            case core::PseudoFSCodes::NOT_FOUND: 
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This folder already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("Folder creation operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_deleteFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        int code;
        core::PseudoFS::getFolderData(where, code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("Folder not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        else if (core::UserManager::currentUserData().getPermissions() < Permissions::admin && core::PseudoFS::getFolderData(where, code).system) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        core::PseudoFS::deleteFolder(where);
        core::PseudoFS::savePFS();
    }
}

void core::commands::CORE_COMMAND_renameFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return;
    }
    else {
        int code;
        core::PseudoFS::getFolderData(args.at(0), code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("Folder not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        else if (core::UserManager::currentUserData().getPermissions() < Permissions::admin && core::PseudoFS::getFolderData(args.at(0), code).system) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }

        switch (core::PseudoFS::renameFolder(args.at(0), args.at(1))) {
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This folder already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("The folder renaming operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_moveFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print("Command error: One of the arguments is empty!\n", core::PrintColors::red);
        return;
    }
    else {
        int code;
        core::PseudoFS::getFolderData(args.at(0), code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("Folder not found!\n", core::PrintColors::red);
            return;
        }    

        if (core::UserManager::currentUserData().getPermissions() < Permissions::user) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        else if (core::UserManager::currentUserData().getPermissions() < Permissions::admin && core::PseudoFS::getFolderData(args.at(0), code).system) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }

        switch (core::PseudoFS::moveFolder(args.at(0), args.at(1))) {
            case core::PseudoFSCodes::NOT_FOUND:
                core::print("Folder not found!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::ALREADY_EXISTS:
                core::print("This folder already exists!\n", core::PrintColors::red);
                break;
            case core::PseudoFSCodes::PATH_IS_EMPTY:
                core::print("Folder move operation was not completed - PATH IS EMPTY!\n", core::PrintColors::red);
                break;
            default:
                core::PseudoFS::savePFS();
                break;
        }
    }
}

void core::commands::CORE_COMMAND_showFolderData(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print("Command error: The first argument cannot be empty!\n", core::PrintColors::red);
        return;
    }
    else {
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : core::PseudoFS::getCurrentPath() + args.at(0));
        int code;
        core::PseudoFS::getFolderData(where, code);
        if (code == core::PseudoFSCodes::NOT_FOUND) {
            core::print("Folder not found!\n", core::PrintColors::red);
            return;
        }
        
        if (core::UserManager::currentUserData().getPermissions() < Permissions::user && (core::PseudoFS::getFolderData(where, code).hidden || core::PseudoFS::getFolderData(where, code).system)) {
            core::print("Error: Not enough permissions to perform this action.\n", core::PrintColors::red);
            return;
        }
        
        core::print("Folder name: " + core::PseudoFS::getFolderData(where, code).name + "\n", core::PrintColors::aqua);
        core::print("Folder ID: " + std::to_string(core::PseudoFS::getFolderData(where, code).id) + "\n", core::PrintColors::aqua);
    }
}

void core::commands::CORE_COMMAND_dir(const std::vector<std::string>& args) {
    bool showHidden = (args.empty() ? false : args.at(0) == "h");
    int fCount = 0;
    int code;
    const std::vector<FolderData> folders = (core::PseudoFS::getCurrentPath() == "./" ? core::PseudoFS::getNRFS().getRoot().folders : core::PseudoFS::getFolderData(core::PseudoFS::getCurrentPath(), code).folders);
    const std::vector<FileData> files = (core::PseudoFS::getCurrentPath() == "./" ? core::PseudoFS::getNRFS().getRoot().files : core::PseudoFS::getFolderData(core::PseudoFS::getCurrentPath(), code).files);
    for (core::FolderData folder : folders) {
        if (folder.hidden && showHidden) {
            core::print(folder.name + "    " + (fCount >= 5 ? "\n" : ""), core::PrintColors::light_aqua);
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
        else if (!folder.hidden) {
            std::cout << folder.name + "    " + (fCount >= 5 ? "\n" : "");
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
    }
    for (core::FileData file : files) {
        if (file.hidden && showHidden) {
            core::print(file.name + "    " + (fCount >= 5 ? "\n" : ""), core::PrintColors::light_aqua);
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
        else if (!file.hidden) {
            std::cout << file.name + "    " + (fCount >= 5 ? "\n" : "");
            if (fCount >= 5) fCount = 0;
            else ++fCount;
        }
    }
    std::cout << '\n';
}

void core::commands::CORE_COMMAND_tree(const std::vector<std::string>& args) {
    if (args.empty())
        core::PseudoFS::showTree(false, false);
    else
        core::PseudoFS::showTree((args.size() > 1 ? (args.at(1) == "h" ? true : args.at(1) == "fh") : false), (args.size() > 1 ? (args.at(1) == "f" ? true : args.at(1) == "fh") : false), args.at(0));
}

void core::commands::CORE_COMMAND_showAll(const std::vector<std::string>& args) {
    if (args.empty())
        core::PseudoFS::printAll(false);
    else 
        core::PseudoFS::printAll((args.size() > 1 ? args.at(1) == "h" : false), args.at(0));
}

void core::commands::CORE_COMMAND_searchFile(const std::vector<std::string>& args) {
    if (args.empty()) return;
    std::string path = (args.size() > 1 ? args.at(1) : core::PseudoFS::getCurrentPath());
    bool fullSearch = args.size() < 2;
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    for (const FileData& file : core::PseudoFS::getNRFS().getRoot().files) {
        if (file.name.find(args.at(0)) < LONG_LONG_MAX) {
            std::cout << "./" + file.name << '\n';
        }
    }

    for (const FolderData& folder : core::PseudoFS::getNRFS().getRoot().folders) {
        if (fullSearch) {
            core::commands::CORE_COMMAND_searchFileHelper(folder, args.at(0), {}, "./" + folder.name);
        }
        else {
            parsedPath.erase(parsedPath.begin());
            for (FolderData& folder : core::PseudoFS::getNRFS().getRoot().folders) {
                if (parsedPath.at(0) == folder.name) {
                    core::commands::CORE_COMMAND_searchFileHelper(folder, args.at(0), parsedPath, "./" + folder.name);
                }
            }
        }
    }
}

void core::commands::CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath) {
    bool fullSearch = path.empty();
    if (curFolder.files.empty() && curFolder.folders.empty()) {
        return;
    }
    for (const core::FileData& file : curFolder.files) {
        if (file.name.find(what) < LONG_LONG_MAX) {
            std::cout << stringPath + "/" + file.name << '\n';
        }
    }

    for (const core::FolderData& folder : curFolder.folders) {
        if (fullSearch) {
            core::commands::CORE_COMMAND_searchFileHelper(folder, what, {}, stringPath + "/" + folder.name);
        }
        else {
            path.erase(path.begin());
            for (FolderData& folder : core::PseudoFS::getNRFS().getRoot().folders) {
                if (path.at(0) == folder.name) {
                    core::commands::CORE_COMMAND_searchFileHelper(folder, what, path, stringPath + "./" + folder.name);
                }
            }
        }
    }
}

void core::commands::CORE_COMMAND_whereIm() {
    std::vector<std::string> parsedPath = core::Utils::split(core::PseudoFS::getCurrentPath(), '/');
    core::print("You're in the folder: " + parsedPath.back() + "\n", core::PrintColors::aqua);
    core::print("Full path: " + core::PseudoFS::getCurrentPath() + "\n", core::PrintColors::aqua);
}


//   -------------- Users "Manager" ---------------

void core::commands::CORE_COMMAND_setPassword(const std::vector<std::string>& args) {
    if (core::UserManager::havePassword(core::UserManager::yourUsername())) {
        if (!core::UserManager::currentUserData().truePassword(args[0])) {
            print("Wrong password!\n", core::PrintColors::red);
            return;
        }
    }
    core::UserManager::currentUserData().editPassword((core::UserManager::havePassword(core::UserManager::yourUsername()) ? args[1] : args[0]));
    core::UserManager::saveUserData(core::UserManager::yourUsername());
}

void core::commands::CORE_COMMAND_editDisplayName(const std::vector<std::string>& args) {
    core::UserManager::currentUserData().editDisplayName(args.at(0));
    core::UserManager::saveUserData(core::UserManager::yourUsername());
}

void core::commands::CORE_COMMAND_createUser(const std::vector<std::string>& args) {
    if (!core::UserManager::userExist(args.at(0))) {
        core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
        return;
    }
    else if (args.at(1).empty()) {
        core::print("COMMAND WARNING: A user will be created with \"User\" permissions\n", core::PrintColors::yellow);
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                return;
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args.at(1))) {
            core::print("COMMAND ERROR\n", core::PrintColors::red);
            return;
        }
    }
    core::UserManager::addUser(args.at(0), static_cast<Permissions>((!args.at(1).empty() ? stoi(args.at(1)) : 0)));
};

void core::commands::CORE_COMMAND_deleteUser(const std::vector<std::string>& args) {
    if (!core::UserManager::userExist(args.at(0))) {
        core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                return;
            }
            pos++;
        }
    }
    core::UserManager::deleteUser(args.at(0));
};

void core::commands::CORE_COMMAND_renameUser(const std::vector<std::string>& args) {
    if (!core::UserManager::userExist(args.at(0))) {
        core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                return;
            }
            pos++;
        }
        pos = 1;
        for (const char& letter : args.at(1)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                return;
            }
            pos++;
        }
    }
    core::UserManager::renameUser(args.at(0), args.at(1));
}

void core::commands::CORE_COMMAND_setPermissionsUser(const std::vector<std::string>& args) {
    if (!core::UserManager::userExist(args.at(0))) {
        core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
                return;
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args.at(1))) {
            core::print("COMMAND ERROR\n", core::PrintColors::red);
            return;
        }
    }
    core::UserManager::changePermissionsUser(args.at(0), static_cast<Permissions>(stoi(args[1])));
};

//void core::commands::CORE_COMMAND_addLocalVar() {}
//void core::commands::CORE_COMMAND_renameLocalVar() {}
//void core::commands::CORE_COMMAND_editLocalVarFunction() {}
//void core::commands::CORE_COMMAND_editLocalVarDescription() {}

//   -------------- Users ---------------

void core::commands::CORE_COMMAND_whoim() {
    if (!core::UserManager::getUserMap()[core::UserManager::yourUsername()].empty()) {
        std::cout << "Username: " << core::UserManager::yourUsername() << '\n';
        std::cout << "Display Name: " << core::UserManager::getUserMap()[core::UserManager::yourUsername()] << '\n';
        std::cout << "Permissions: " << permissionsS(core::UserManager::getPermissionsMap()[core::UserManager::yourUsername()]) << '\n';
    }
};

void core::commands::CORE_COMMAND_infoUser(const std::vector<std::string>& args) {
    if (!core::UserManager::userExist(args.at(0))) {
        core::print("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
        return;
    }
    std::cout << "Username: " << args.at(0) << '\n';
    std::cout << "Display Name: " << core::UserManager::getUserMap()[args.at(0)] << '\n';
    std::cout << "Permissions: " << permissionsS(core::UserManager::getPermissionsMap()[args.at(0)]) << '\n';
};

void core::commands::CORE_COMMAND_allInfoUsers() {
    std::cout << "  - [ All Users Info ] -  " << '\n';
    for (auto& user : core::UserManager::getUserMap()) {
        std::cout << " - " + user.first + " | " + user.second << '\n';
        std::cout << "Language: " << core::UserManager::getLanguageMap()[user.first] << '\n';
        std::cout << "Permissions: " << permissionsS(core::UserManager::getPermissionsMap()[user.first]) << "\n\n";
    }
};

void core::commands::CORE_COMMAND_logout() {
    std::string choice;
    print("Are you sure you want to log out of your current user account? (Y/N): ", core::PrintColors::yellow);
    std::cin >> choice;
    while (true) {
        if (choice == "Y") { core::UserManager::userLogout(); break; }
        else if (choice == "N") { break; }
        else { 
            print("Error. Are you sure you want to log out of the current user account? (Y/N): ", core::PrintColors::red);
            std::cin >> choice;
        }
    }
}

//   -------------- Other ---------------

//void core::commands::CORE_COMMAND_addSystemVar() {}

void core::commands::CORE_COMMAND_allSystemVars() {
    for (auto var : core::SystemVariablesManager::getAllVars()) {
        print(var.first + " - ", core::PrintColors::light_aqua);
        print(var.second + "\n", core::PrintColors::light_blue);
    }
}

void core::commands::CORE_COMMAND_allLocalVars() {
    for (auto var : core::UserManager::getLocalVarsMap(core::UserManager::yourUsername())) {
        print(var.first + " - ", core::PrintColors::light_aqua);
        print(var.second + "\n", core::PrintColors::light_blue);
    }
}