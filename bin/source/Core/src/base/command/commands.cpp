#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "Core/base/print.h"
#include "Core/base/command/commands.h"
#include "Core/base/command/handler_commands.h"
#include "Core/CORE_info.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/filesystem/pseudo_fs.h"

void CORE_COMMAND_help() {
    handlerCommands HC;
    for (auto command : HC.getAllCommands())
        print(print::colors::light_green, command.first + command.second + '\n');
};

void CORE_COMMAND_info() {
    std::cout << CORE_NAME << " by " << CORE_DEVELOPER << '\n' << CORE_VERSION << '\n';
    std::cout << "JSON library by nlohmann - https://github.com/nlohmann/json \n";
    std::cout << "CC (Color Console) by aafulei - https://github.com/aafulei/color-console \n\n";
    std::cout << " --- Special Thanks ---\nAlone Knight - migrating NRC from Makefile to CMake\n";
};

/*
void CORE_COMMAND_exit() {
    work = false;
}
*/

//   -------------- Pseudo FS Commands ---------------

/*
void CORE_COMMAND_cd() {
    pseudoFSBase FS;
    std::string folderPath;
    std::cout << "Enter folder: ";
    std::cin >> folderPath;
    FS.changeDirectory(folderPath);
}
*/

/*
void CORE_COMMAND_tree() {
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

void CORE_COMMAND_addUser() {
    userManager UM;
    std::string username;
    int permissions;
    print(print::colors::aqua, "Enter username: ");
    std::cin >> username;
    print(print::colors::aqua, "Permissions (Ghost (-1), User (0), Admin (1)) | ONLY NUMBERS: ");
    std::cin >> permissions;
    UM.addUser(username, static_cast<permissionsEC>(permissions));
};


void CORE_COMMAND_deleteUser() {
    userManager UM;
    std::string Username;
    print(print::colors::aqua, "Enter username: ");
    std::cin >> Username;
    UM.deleteUser(Username);
};

void CORE_COMMAND_renameUser() {
    userManager UM;
    std::string username, newUsername;
    print(print::colors::aqua, "Enter username: ");
    std::cin >> username;
   print(print::colors::aqua, "Enter new username: ");
    std::cin >> newUsername;
    UM.renameUser(username, newUsername);
}

void CORE_COMMAND_setPermissionsUser() {
    userManager UM;
    std::string username;
    int permissions;
    print(print::colors::aqua, "Enter username: ");
    std::cin >> username;
    print(print::colors::aqua, "Permissions (Ghost (-1), User (0), Admin (1)) | ONLY NUMBERS: ");
    std::cin >> permissions;
    UM.changePermissionsUser(username, static_cast<permissionsEC>(permissions));
};

//void CORE_COMMAND_addLocalVar() {}
//void CORE_COMMAND_renameLocalVar() {}
//void CORE_COMMAND_editLocalVarFunction() {}
//void CORE_COMMAND_editLocalVarDescription() {}

//   -------------- Users ---------------

void CORE_COMMAND_infoUser() {
    userManager UM;
    if (!UM.getUserMap()[UM.yourUsername()].empty()) {
        std::cout << "Username: " << UM.yourUsername() << '\n';
        std::cout << "Display Name: " << UM.getUserMap()[UM.yourUsername()] << '\n';
        std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[UM.yourUsername()]) << '\n';
    }
};

void CORE_COMMAND_allInfoUsers() {
    userManager UM;
    std::cout << "  - [ All Users Info ] -  " << '\n';
    for (auto& user : UM.getUserMap()) {
        std::cout << " - " + user.first + " | " + user.second << '\n';
        std::cout << "Language: " << UM.getLanguageMap()[user.first] << '\n';
        std::cout << "Permissions: " << permissionsS(UM.getPermissionsMap()[user.first]) << "\n\n";
    }
};

void CORE_COMMAND_logout() {
    userManager UM;
    std::string choice;
    print(print::colors::yellow, "Are you sure you want to log out of your current user account? (Y/N): ");
    std::cin >> choice;
    while (true) {
        if (choice == "Y") { UM.userLogout(); break; }
        else if (choice == "N") { break; }
        else { print(print::colors::red, "Error. Are you sure you want to log out of the current user account? (Y/N): "); }
    }
}

//   -------------- Other ---------------

//void CORE_COMMAND_addSystemVar() {}

void CORE_COMMAND_allSystemVars() {
    systemVariables SV;
    for (auto var : SV.getAllVars()) {
        print(print::colors::light_aqua, var.first + " - ");
        print(print::colors::light_blue, var.second + "\n");
    }
}

void CORE_COMMAND_allLocalVars() {
    userManager UM;
    for (auto var : UM.getLocalVarsMap(UM.yourUsername())) {
        print(print::colors::light_aqua, var.first + " - ");
        print(print::colors::light_blue, var.second + "\n");
    }
}