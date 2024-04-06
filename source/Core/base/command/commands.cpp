#pragma once

#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <ctime>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <cctype>
#include <type_traits>
#include <map>
#include "HandlerCommands.cpp"
#include "../../extra/variables.cpp"
#include "../../CORE_info.h"
#include "../../users/user_manager.cpp"
#include "../../filesystem/pseudo_fs.cpp"

void CORE_COMMAND_help() {
    HandlerCommands HC;
    HC.getAllCommands();
};

void CORE_COMMAND_info() {
    std::cout << CORE_NAME << " by " << CORE_DEVELOPER << std::endl;
    std::cout << CORE_VERSION << std::endl;
};

/*
void CORE_COMMAND_tree() {

};
*/

//   -------------- Users "Manager" ---------------

void CORE_COMMAND_addUser() {
    userManager UM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UM.addUser(Username, Permissions);
};


void CORE_COMMAND_deleteUser() {
    userManager UM;
    std::string Username;
    std::cout << "Enter username: ";
    std::cin >> Username;
    UM.deleteUser(Username);
};

void CORE_COMMAND_renameUser() {
    userManager UM;
    std::string Username, New_Username;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Enter new username: ";
    std::cin >> New_Username;
    UM.renameUser(Username, New_Username);
}

void CORE_COMMAND_setPermissionsUser() {
    userManager UM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UM.changePermissionsUser(Username, Permissions);
};

//   -------------- Users ---------------

void CORE_COMMAND_infoUser() {
    userManager UM;
    if (!UM.getUserInfo(UM.YourUsername()).empty()) {
        std::cout << "Username: " << UM.getUserInfo(UM.YourUsername())[0] << std::endl;
        std::cout << "Permissions: " << UM.getUserInfo(UM.YourUsername())[1] << std::endl;
    }
};

void CORE_COMMAND_allInfoUsers() {
    userManager UM;
    UM.getAllInfoUsers();
};

void CORE_COMMAND_logout() {
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