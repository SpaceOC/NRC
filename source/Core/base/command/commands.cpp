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

void CORE_help() {
    HandlerCommands HC;
    HC.getAllCommands();
};

void CORE_info() {
    std::cout << CORE_NAME << " by " << CORE_DEVELOPER << std::endl;
    std::cout << CORE_VERSION << std::endl;
};

/*
void CORE_tree() {

};
*/

void CORE_info_user() {
    userManager UserM;
    for (auto element : UserM.getInfoUser(UserM.YourUsername())) {
        std::cout << element << std::endl;
    }
};

void CORE_all_info_users() {
    userManager UserM;
    UserM.getAllInfoUsers();
};

void CORE_add_user() {
    userManager UserM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UserM.addUser(Username, Permissions);
};


void CORE_delete_user() {
    userManager UserM;
    std::string Username;
    std::cout << "Enter username: ";
    std::cin >> Username;
    UserM.deleteUser(Username);
};

void CORE_rename_user() {

}

void CORE_set_permissions_user() {
    userManager UsersM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UsersM.changePermissionsUser(Username, Permissions);
};