#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "Core/base/command/commands.h"
#include "Core/base/command/handler_commands.h"
#include "Core/CORE_info.h"
#include "Core/users/user_manager.h"
#include "Core/filesystem/pseudo_fs.h"

void CORE_COMMAND_help(std::vector<std::string> vars) {
    HandlerCommands HC;
    HC.getAllCommands();
};

void CORE_COMMAND_info(std::vector<std::string> vars) {
    std::cout << CORE_NAME << " by " << CORE_DEVELOPER << std::endl;
    std::cout << CORE_VERSION << std::endl;
};

//   -------------- Pseudo FS Commands ---------------

void CORE_COMMAND_cd(std::vector<std::string> vars) {
    /*s
    pseudoFSBase FS;
    std::string folderPath;
    std::cout << "Enter folder: ";
    std::cin >> folderPath;
    FS.changeDirectory(folderPath);
    */
}

void CORE_COMMAND_tree(std::vector<std::string> vars) {
    /*
    pseudoFSBase FS;
    if (!FS.getCurrentFSList().empty()) {
        for (auto element : FS.getCurrentFSList()) {
            std::cout << element << std::endl;
        }
    }
    else {
        std::cout << "No folders/files" << std::endl;
    }
    */
}

//   -------------- Users "Manager" ---------------

void CORE_COMMAND_addUser(std::vector<std::string> vars) {
    userManager UM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UM.addUser(Username, Permissions);
};


void CORE_COMMAND_deleteUser(std::vector<std::string> vars) {
    userManager UM;
    std::string Username;
    std::cout << "Enter username: ";
    std::cin >> Username;
    UM.deleteUser(Username);
};

void CORE_COMMAND_renameUser(std::vector<std::string> vars) {
    userManager UM;
    std::string Username, New_Username;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Enter new username: ";
    std::cin >> New_Username;
    UM.renameUser(Username, New_Username);
}

void CORE_COMMAND_setPermissionsUser(std::vector<std::string> vars) {
    userManager UM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UM.changePermissionsUser(Username, Permissions);
};

//   -------------- Users ---------------

void CORE_COMMAND_infoUser(std::vector<std::string> vars) {
    userManager UM;
    if (!UM.getUserInfo(UM.YourUsername()).empty()) {
        std::cout << "Username: " << UM.getUserInfo(UM.YourUsername())[0] << std::endl;
        std::cout << "Permissions: " << UM.getUserInfo(UM.YourUsername())[1] << std::endl;
    }
};

void CORE_COMMAND_allInfoUsers(std::vector<std::string> vars) {
    userManager UM;
    std::cout << "  - [ All Users Info ] -  " << std::endl;
    std::map<std::string, std::map<std::string, std::string>> temp = {
        {"lang", UM.getLanguageMap()},
        {"permissions", UM.getPermissionsMap()}
    };
    for (auto& user : UM.getUserMap()) {
        std::cout << " - " + user.first << std::endl;
        std::cout << "Language: " << temp["lang"][user.first] << std::endl;
        std::cout << "Permissions: " << temp["permissions"][user.first] << std::endl << std::endl;
    }
};

void CORE_COMMAND_logout(std::vector<std::string> vars) {
    userManager UM;
    std::string choice;
    std::cout << "Are you sure you want to log out of your current user account? (Y/N): ";
    std::cin >> choice;
    while (true) {
        if (choice == "Y") { UM.userLogout(); break; }
        else if (choice == "N") { break; }
        else { std::cout << "Error. Are you sure you want to log out of the current user account? (Y/N): "; }
    }
}