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
    HandlerCommands HC;
    if (args.empty()) {
        for (const auto& command : HC.getAllCommands()) {
            std::string argsNames = "";
            if (!command.second.argsNames.empty()) {
                for (auto arg : command.second.argsNames) {
                    argsNames += " <" + arg + ">";
                }
            }
            print(colors::light_green, command.first + argsNames + command.second.description + '\n');
        }
    }
    else {
        if (HC.getCommand(args.at(0)).empty())
            core::print("Command not found\n");
        else {
            std::map<std::string, core::CommandDescription> temp = HC.getCommand(args.at(0));
            std::string argsNames = "";
            if (!temp[args.at(0)].argsNames.empty()) {
                for (auto arg : temp[args.at(0)].argsNames) {
                    argsNames += " <" + arg + ">";
                }
            }
            print(colors::light_green, args.at(0) + argsNames + temp[args.at(0)].description + '\n');
        }
    }
};

void core::commands::CORE_COMMAND_info() {
    std::cout << CORE_NAME << " by " << CORE_DEVELOPER << '\n' << CORE_VERSION << " " << CORE_VERSION_TYPE << '\n';
    std::cout << "JSON library by nlohmann - https://github.com/nlohmann/json \n";
    std::cout << " --- Special Thanks ---\nAlone Knight - migrating NRC from Makefile to CMake\n";
};

void core::commands::CORE_COMMAND_time(const std::vector<std::string>& args) {
    HandlerCommands HC;
    auto start = std::chrono::steady_clock::now();
    std::string temp;

    for (const std::string& anotherTemp : args)
        temp += (anotherTemp == HC.getCommandSeparator() ? "" : "\"") + anotherTemp + (anotherTemp != args.back() ? (anotherTemp == HC.getCommandSeparator() ? "" : "\" ") : (anotherTemp == HC.getCommandSeparator() ? "" : "\""));
    
    core::CommandObject command = HC.parsing(temp).at(0);
    HC.sendCommand(command);
    auto end = std::chrono::steady_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    core::print(core::colors::light_green, "time: ");
    std::cout << duration;
    core::print(core::colors::light_green, "ms\n");
}

/*
void core::commands:: core::commands:: CORE_COMMAND_exit() {
    work = false;
}
*/

//   -------------- Pseudo FS Commands ---------------


void core::commands::CORE_COMMAND_cd(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        PseudoFS FS;
        std::string where = (args.at(0) == ".." && !args.at(0)._Starts_with("./") ? ".." : args.at(0));
        if (where == ".." || !args.at(0)._Starts_with("./")) {
            if (!FS.changeDirectory(where)) {
                core::print(core::red, "Error: Change directory operation failed!\n");
            }
        }
        else {
            if (!FS.changePath(where)) {
                core::print(core::red, "Error: Change path operation failed!\n");
            }
        }
    }
}

void core::commands::CORE_COMMAND_createFile(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : FS.getCurrentPath() + args.at(0));
        FS.createFile(where);
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_deleteFile(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : FS.getCurrentPath() + args.at(0));
        if (FS.getFileData(where).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        else if (UM.currentUserData().getPermissions() < permissionsEC::admin && FS.getFileData(where).system) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }

        FS.deleteFile(where);
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_renameFile(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print(core::red, "Command error: One of the arguments is empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        if (FS.getFileData(args.at(0)).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        else if (UM.currentUserData().getPermissions() < permissionsEC::admin && FS.getFileData(args.at(0)).system) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }

        FS.renameFile(args.at(0), args.at(1));
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_moveFile(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print(core::red, "Command error: One of the arguments is empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        if (FS.getFileData(args.at(0)).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        else if (UM.currentUserData().getPermissions() < permissionsEC::admin && FS.getFileData(args.at(0)).system) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }

        FS.moveFile(args.at(0), args.at(1));
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_showFileData(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : FS.getCurrentPath() + args.at(0));
        if (FS.getFileData(where).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user && (FS.getFileData(where).hidden || FS.getFileData(where).system)) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        
        core::print(core::aqua, "File name: " + FS.getFileData(where).name + "\n");
        core::print(core::aqua, "File content: " + FS.getFileData(where).content + "\n");
        core::print(core::aqua, "File ID: " + std::to_string(FS.getFileData(where).id) + "\n");
    }
}

void core::commands::CORE_COMMAND_createFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : FS.getCurrentPath() + args.at(0));
        FS.createFolder(where);
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_deleteFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : FS.getCurrentPath() + args.at(0));
        if (FS.getFolderData(where).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        else if (UM.currentUserData().getPermissions() < permissionsEC::admin && FS.getFolderData(where).system) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        FS.deleteFolder(where);
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_renameFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print(core::red, "Command error: One of the arguments is empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        if (FS.getFolderData(args.at(0)).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        else if (UM.currentUserData().getPermissions() < permissionsEC::admin && FS.getFolderData(args.at(0)).system) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }

        FS.renameFolder(args.at(0), args.at(1));
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_moveFolder(const std::vector<std::string>& args) {
    if (args.at(0).empty() || args.at(1).empty()) {
        core::print(core::red, "Command error: One of the arguments is empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        if (FS.getFolderData(args.at(0)).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        else if (UM.currentUserData().getPermissions() < permissionsEC::admin && FS.getFolderData(args.at(0)).system) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }

        FS.moveFolder(args.at(0), args.at(1));
        FS.savePFS();
    }
}

void core::commands::CORE_COMMAND_showFolderData(const std::vector<std::string>& args) {
    if (args.at(0).empty()) {
        core::print(core::red, "Command error: The first argument cannot be empty!\n");
        return;
    }
    else {
        UserManager UM;
        PseudoFS FS;
        std::string where = (args.at(0)._Starts_with("./") ? args.at(0) : FS.getCurrentPath() + args.at(0));
        if (FS.getFolderData(where).name.empty()) return;
        
        if (UM.currentUserData().getPermissions() < permissionsEC::user && (FS.getFolderData(where).hidden || FS.getFolderData(where).system)) {
            core::print(core::red, "Error: Not enough permissions to perform this action.\n");
            return;
        }
        
        core::print(core::aqua, "Folder name: " + FS.getFolderData(where).name + "\n");
        core::print(core::aqua, "Folder ID: " + std::to_string(FS.getFolderData(where).id) + "\n");
    }
}

void core::commands::CORE_COMMAND_NRFSSize() {
    PseudoFS FS;
    FS.printDiskSize();
}

void core::commands::CORE_COMMAND_dir(const std::vector<std::string>& args) {
    PseudoFS FS;
    bool showHidden = (args.empty() ? false : args.at(0) == "h");
    int fCount = 0;
    const std::vector<FolderData> folders = (FS.getCurrentPath() == "./" ? FS.getNRFS().getRoot().folders : FS.getFolderData(FS.getCurrentPath()).folders);
    const std::vector<FileData> files = (FS.getCurrentPath() == "./" ? FS.getNRFS().getRoot().files : FS.getFolderData(FS.getCurrentPath()).files);
    for (core::FolderData folder : folders) {
        if (folder.hidden && showHidden) {
            core::print(core::light_aqua, folder.name + "    " + (fCount >= 5 ? "\n" : ""));
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
            core::print(core::light_aqua, file.name + "    " + (fCount >= 5 ? "\n" : ""));
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
    if (args.empty()) {
        PseudoFS FS;
        FS.showTree(false, false);
    }
    else {
        PseudoFS FS;
        FS.showTree((args.size() > 1 ? (args.at(1) == "h" ? true : args.at(1) == "fh") : false), (args.size() > 1 ? (args.at(1) == "f" ? true : args.at(1) == "fh") : false), args.at(0));
    }
}

void core::commands::CORE_COMMAND_showAll(const std::vector<std::string>& args) {
    if (args.empty()) {
        PseudoFS FS;
        FS.printAll(false);
    }
    else {
        PseudoFS FS;
        FS.printAll((args.size() > 1 ? args.at(1) == "h" : false), args.at(0));
    }
}

void core::commands::CORE_COMMAND_searchFile(const std::vector<std::string>& args) {
    if (args.empty()) return;
    PseudoFS FS;
    std::string path = (args.size() > 1 ? args.at(1) : FS.getCurrentPath());
    bool fullSearch = args.size() < 2;
    std::vector<std::string> parsedPath = core::Utils::split(path, '/');
    for (const FileData& file : FS.getNRFS().getRoot().files) {
        if (file.name.find(args.at(0)) < LONG_LONG_MAX) {
            std::cout << "./" + file.name << '\n';
        }
    }

    for (const FolderData& folder : FS.getNRFS().getRoot().folders) {
        if (fullSearch) {
            core::commands::CORE_COMMAND_searchFileHelper(folder, args.at(0), {}, "./" + folder.name);
        }
        else {
            parsedPath.erase(parsedPath.begin());
            for (FolderData& folder : FS.getNRFS().getRoot().folders) {
                if (parsedPath.at(0) == folder.name) {
                    core::commands::CORE_COMMAND_searchFileHelper(folder, args.at(0), parsedPath, "./" + folder.name);
                }
            }
        }
    }
}

void core::commands::CORE_COMMAND_searchFileHelper(const core::FolderData& curFolder, const std::string& what, std::vector<std::string> path, std::string stringPath) {
    core::PseudoFS FS;
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
            for (FolderData& folder : FS.getNRFS().getRoot().folders) {
                if (path.at(0) == folder.name) {
                    core::commands::CORE_COMMAND_searchFileHelper(folder, what, path, stringPath + "./" + folder.name);
                }
            }
        }
    }
}

void core::commands::CORE_COMMAND_whereIm() {
    PseudoFS FS;
    std::vector<std::string> parsedPath = core::Utils::split(FS.getCurrentPath(), '/');
    core::print(core::aqua, "You're in the folder: " + parsedPath.back() + "\n");
    core::print(core::aqua, "Full path: " + FS.getCurrentPath() + "\n");
}


//   -------------- Users "Manager" ---------------

void core::commands::CORE_COMMAND_setPasswordWithDialogue() {
    UserManager UM;
    if (UM.havePassword(UM.yourUsername())) {
        std::string oldPassword;
        print(colors::aqua, "Enter (old) password: ");
        while (!(std::cin >> std::ws)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::getline(std::cin, oldPassword);
        if (!UM.currentUserData().truePassword(oldPassword)) {
            print(colors::red, "Wrong password!\n");
            return;
        }
    }

    std::string password;
    print(colors::aqua, "Enter new password: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, password);
    UM.currentUserData().editPassword(password);
    UM.saveUserData(UM.yourUsername());
}

void core::commands::CORE_COMMAND_setPassword(const std::vector<std::string>& args) {
    UserManager UM;
    if (args.empty()) {
        CORE_COMMAND_setPasswordWithDialogue();
        return;
    }
    else if (UM.havePassword(UM.yourUsername())) {
        if (!UM.currentUserData().truePassword(args[0])) {
            print(colors::red, "Wrong password!\n");
            return;
        }
    }
    UM.currentUserData().editPassword((UM.havePassword(UM.yourUsername()) ? args[1] : args[0]));
    UM.saveUserData(UM.yourUsername());
}

void core::commands::CORE_COMMAND_editDisplayNameWithDialogue() {
    UserManager UM;
    std::string displayName;
    print(colors::aqua, "Enter new Display Name: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, displayName);
    UM.currentUserData().editDisplayName(displayName);
    UM.saveUserData(UM.yourUsername());
}

void core::commands::CORE_COMMAND_editDisplayName(const std::vector<std::string>& args) {
    if (args.empty()) {
        CORE_COMMAND_editDisplayNameWithDialogue();
        return;
    }
    UserManager UM;
    UM.currentUserData().editDisplayName(args.at(0));
    UM.saveUserData(UM.yourUsername());
}

void core::commands::CORE_COMMAND_addUserWithDialogue() {
    UserManager UM;
    std::string username;
    int permissions;
    print(colors::aqua, "Enter username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, username);
    print(colors::aqua, "Permissions (Ghost (-1), User (0), Admin (1)) | ONLY NUMBERS: ");
    std::cin >> permissions;
    UM.addUser(username, static_cast<permissionsEC>(permissions));
};

void core::commands::CORE_COMMAND_addUser(const std::vector<std::string>& args) {
    UserManager UM;
    if (args.empty()) {
        CORE_COMMAND_addUserWithDialogue();
        return;
    }
    if (!UM.userExist(args.at(0))) {
        core::print(core::colors::red, "COMMAND ERROR: The user doesn't exist!\n");
        return;
    }
    else if (args.at(1).empty()) {
        core::print(core::colors::yellow, "COMMAND WARNING: A user will be created with \"User\" permissions\n");
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args.at(1))) {
            core::print(core::colors::red, "COMMAND ERROR\n");
            return;
        }
    }
    UM.addUser(args.at(0), static_cast<permissionsEC>((!args.at(1).empty() ? stoi(args.at(1)) : 0)));
};

void core::commands::CORE_COMMAND_deleteUserWithDialogue() {
    UserManager UM;
    std::string username;
    print(colors::aqua, "Enter username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, username);
    UM.deleteUser(username);
};

void core::commands::CORE_COMMAND_deleteUser(const std::vector<std::string>& args) {
    UserManager UM;
    if (args.empty()) {
        CORE_COMMAND_deleteUserWithDialogue();
        return;
    }
    if (!UM.userExist(args.at(0))) {
        core::print(core::colors::red, "COMMAND ERROR: The user doesn't exist!\n");
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
    }
    UM.deleteUser(args.at(0));
};

void core::commands::CORE_COMMAND_renameUserWithDialogue() {
    UserManager UM;
    std::string username, newUsername;
    print(colors::aqua, "Enter username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, username);
    print(colors::aqua, "Enter new username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, newUsername);
    UM.renameUser(username, newUsername);
}

void core::commands::CORE_COMMAND_renameUser(const std::vector<std::string>& args) {
    UserManager UM;
    if (args.empty()) {
        CORE_COMMAND_renameUserWithDialogue();
        return;
    }
    if (!UM.userExist(args.at(0))) {
        core::print(core::colors::red, "COMMAND ERROR: The user doesn't exist!\n");
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
        pos = 1;
        for (const char& letter : args.at(1)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
    }
    UM.renameUser(args.at(0), args.at(1));
}

void core::commands::CORE_COMMAND_setPermissionsUserWithDialogue() {
    UserManager UM;
    std::string username;
    int permissions;
    print(colors::aqua, "Enter username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, username);
    print(colors::aqua, "Permissions (Ghost (-1), User (0), Admin (1)) | ONLY NUMBERS: ");
    std::cin >> permissions;
    UM.changePermissionsUser(username, static_cast<permissionsEC>(permissions));
};

void core::commands::CORE_COMMAND_setPermissionsUser(const std::vector<std::string>& args) {
    UserManager UM;
    if (args.empty()) {
        CORE_COMMAND_setPermissionsUserWithDialogue();
        return;
    }
    if (!UM.userExist(args.at(0))) {
        core::print(core::colors::red, "COMMAND ERROR: The user doesn't exist!\n");
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args.at(0)) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args.at(1))) {
            core::print(core::colors::red, "COMMAND ERROR\n");
            return;
        }
    }
    UM.changePermissionsUser(args.at(0), static_cast<permissionsEC>(stoi(args[1])));
};

//void core::commands::CORE_COMMAND_addLocalVar() {}
//void core::commands::CORE_COMMAND_renameLocalVar() {}
//void core::commands::CORE_COMMAND_editLocalVarFunction() {}
//void core::commands::CORE_COMMAND_editLocalVarDescription() {}

//   -------------- Users ---------------

void core::commands::CORE_COMMAND_whoim() {
    UserManager UM;
    if (!UM.getUserMap()[UM.yourUsername()].empty()) {
        std::cout << "Username: " << UM.yourUsername() << '\n';
        std::cout << "Display Name: " << UM.getUserMap()[UM.yourUsername()] << '\n';
        std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[UM.yourUsername()]) << '\n';
    }
};

void core::commands::CORE_COMMAND_infoUser(const std::vector<std::string>& args) {
    UserManager UM;
    if (!UM.userExist(args.at(0))) {
        core::print(core::colors::red, "COMMAND ERROR: The user doesn't exist!\n");
        return;
    }
    std::cout << "Username: " << args.at(0) << '\n';
    std::cout << "Display Name: " << UM.getUserMap()[args.at(0)] << '\n';
    std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[args.at(0)]) << '\n';
};

void core::commands::CORE_COMMAND_allInfoUsers() {
    UserManager UM;
    std::cout << "  - [ All Users Info ] -  " << '\n';
    for (auto& user : UM.getUserMap()) {
        std::cout << " - " + user.first + " | " + user.second << '\n';
        std::cout << "Language: " << UM.getLanguageMap()[user.first] << '\n';
        std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[user.first]) << "\n\n";
    }
};

void core::commands::CORE_COMMAND_logout() {
    UserManager UM;
    std::string choice;
    print(colors::yellow, "Are you sure you want to log out of your current user account? (Y/N): ");
    std::cin >> choice;
    while (true) {
        if (choice == "Y") { UM.userLogout(); break; }
        else if (choice == "N") { break; }
        else { 
            print(colors::red, "Error. Are you sure you want to log out of the current user account? (Y/N): ");
            std::cin >> choice;
        }
    }
}

//   -------------- Other ---------------

//void core::commands::CORE_COMMAND_addSystemVar() {}

void core::commands::CORE_COMMAND_allSystemVars() {
    SystemVariablesManager SV;
    for (auto var : SV.getAllVars()) {
        print(colors::light_aqua, var.first + " - ");
        print(colors::light_blue, var.second + "\n");
    }
}

void core::commands::CORE_COMMAND_allLocalVars() {
    UserManager UM;
    for (auto var : UM.getLocalVarsMap(UM.yourUsername())) {
        print(colors::light_aqua, var.first + " - ");
        print(colors::light_blue, var.second + "\n");
    }
}