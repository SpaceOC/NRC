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
    handlerCommands HC;
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
    handlerCommands HC;
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

/*
void core::commands:: core::commands:: CORE_COMMAND_cd() {
    pseudoFSBase FS;
    std::string folderPath;
    std::cout << "Enter folder: ";
    std::cin >> folderPath;
    FS.changeDirectory(folderPath);
}
*/

/*
void core::commands:: core::commands:: CORE_COMMAND_tree() {
    pseudoFSBase FS;
    if (!FS.getCurrentFSList().empty()) {
        for (auto element : FS.getCurrentFSList()) {
            std::cout << element << std::endl;
        }
    }
    else {
        std::cout << "No folders/files" << std::endl;
    }
}
*/

//   -------------- Users "Manager" ---------------

void core::commands::CORE_COMMAND_setPasswordWithDialogue() {
    userManager UM;
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
    userManager UM;
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
    userManager UM;
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
    userManager UM;
    UM.currentUserData().editDisplayName(args[0]);
    UM.saveUserData(UM.yourUsername());
}

void core::commands::CORE_COMMAND_addUserWithDialogue() {
    userManager UM;
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
    if (args.empty()) {
        CORE_COMMAND_addUserWithDialogue();
        return;
    }
    else if (args[1].empty()) {
        core::print(core::colors::red, "COMMAND WARNING: A user will be created with \"User\" permissions\n");
    }
    else {
        long long int pos = 1;
        for (const char& letter : args[0]) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args[1])) {
            core::print(core::colors::red, "COMMAND ERROR\n");
            return;
        }
    }
    userManager UM;
    UM.addUser(args[0], static_cast<permissionsEC>((!args[1].empty() ? stoi(args[1]) : 0)));
};

void core::commands::CORE_COMMAND_deleteUserWithDialogue() {
    userManager UM;
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
    if (args.empty()) {
        CORE_COMMAND_deleteUserWithDialogue();
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args[0]) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
    }
    userManager UM;
    UM.deleteUser(args[0]);
};

void core::commands::CORE_COMMAND_renameUserWithDialogue() {
    userManager UM;
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
    if (args.empty()) {
        CORE_COMMAND_renameUserWithDialogue();
        return;
    }
    else if (args[1].empty()) {
        core::print(core::colors::red, "COMMAND ERROR: \n");
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args[0]) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
        pos = 1;
        for (const char& letter : args[1]) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
    }
    userManager UM;
    UM.renameUser(args[0], args[1]);
}

void core::commands::CORE_COMMAND_setPermissionsUserWithDialogue() {
    userManager UM;
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
    if (args.empty()) {
        CORE_COMMAND_setPermissionsUserWithDialogue();
        return;
    }
    else if (args[1].empty()) {
        core::print(core::colors::red, "COMMAND ERROR: \n");
        return;
    }
    else {
        long long int pos = 1;
        for (const char& letter : args[0]) {
            if (ispunct(letter) || letter == ',' || letter == '.') {
                core::print(core::colors::red, "COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n");
                return;
            }
            pos++;
        }
        if (!core::Utils::stringIsNumbers(args[1])) {
            core::print(core::colors::red, "COMMAND ERROR\n");
            return;
        }
    }
    userManager UM;
    UM.changePermissionsUser(args[0], static_cast<permissionsEC>(stoi(args[1])));
};

//void core::commands::CORE_COMMAND_addLocalVar() {}
//void core::commands::CORE_COMMAND_renameLocalVar() {}
//void core::commands::CORE_COMMAND_editLocalVarFunction() {}
//void core::commands::CORE_COMMAND_editLocalVarDescription() {}

//   -------------- Users ---------------

void core::commands::CORE_COMMAND_whoim() {
    userManager UM;
    if (!UM.getUserMap()[UM.yourUsername()].empty()) {
        std::cout << "Username: " << UM.yourUsername() << '\n';
        std::cout << "Display Name: " << UM.getUserMap()[UM.yourUsername()] << '\n';
        std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[UM.yourUsername()]) << '\n';
    }
};

void core::commands::CORE_COMMAND_infoUser(const std::vector<std::string>& args) {
    userManager UM;
    if (args.empty()) {
        core::print(core::colors::red, "COMMAND ERROR: \n");
        return;
    }
    else if (!UM.userExist(args[0])) {
        core::print(core::colors::red, "COMMAND ERROR: \n");
        return;
    }
    std::cout << "Username: " << args[0] << '\n';
    std::cout << "Display Name: " << UM.getUserMap()[args[0]] << '\n';
    std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[args[0]]) << '\n';
};

void core::commands::CORE_COMMAND_allInfoUsers() {
    userManager UM;
    std::cout << "  - [ All Users Info ] -  " << '\n';
    for (auto& user : UM.getUserMap()) {
        std::cout << " - " + user.first + " | " + user.second << '\n';
        std::cout << "Language: " << UM.getLanguageMap()[user.first] << '\n';
        std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[user.first]) << "\n\n";
    }
};

void core::commands::CORE_COMMAND_logout() {
    userManager UM;
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
    systemVariables SV;
    for (auto var : SV.getAllVars()) {
        print(colors::light_aqua, var.first + " - ");
        print(colors::light_blue, var.second + "\n");
    }
}

void core::commands::CORE_COMMAND_allLocalVars() {
    userManager UM;
    for (auto var : UM.getLocalVarsMap(UM.yourUsername())) {
        print(colors::light_aqua, var.first + " - ");
        print(colors::light_blue, var.second + "\n");
    }
}