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
#include "../CORE_info.h"
#include "../users/user_manager.cpp"

// команда help - показывает все команды
void CORE_help() {
    std::vector<std::string> Commands_List = { 
        "- help                 \t Show list commands",
        "- CORE_version         \t Core info",
        //"- tree                 \t W.I.P.",
        "- info_user            \t User Information",
        "- add_user             \t Add User",
        "- all_info_users       \t ..."
        "- delete_user          \t Delete User",
        "- set_permissions_user \t Set User Permissions"
    };
    for (auto command : Commands_List) {
        std::cout << command << "\n";
    }
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
    std::cout << Current_User << std::endl;
    std::cout << "Permissions - " << Users_Permissions[Current_User] << std::endl;
    std::cout << "Language - " << Users_Language[Current_User] << std::endl;
};

void CORE_all_info_users() {
    std::cout << " Users " << std::endl;
    for (auto& element : Users) {
        std::cout << " - " + element << '\n';
    }
    std::cout << " Users Permissions " << std::endl;
    for (auto& elements : Users_Permissions) {
        std::cout << " - " + elements.first + " - " + elements.second << '\n';
    }
    std::cout << " Users Language " << std::endl;
    for (auto& elements : Users_Language) {
        std::cout << " - " + elements.first + " - " + elements.second << '\n';
    }
};

void CORE_add_user() {
    UsersManager UsersM;

    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UsersM.addUser(Username, "English", Permissions);
};

/*
void CORE_delete_user() {
    UsersManager UsersM;
    std::string Username;
    std::cout << "Enter username: ";
    std::cin >> Username;
    UsersM.removeUser(Username);
};
*/

void CORE_set_permissions_user() {
    UsersManager UsersM;
    std::string Username, Permissions;
    std::cout << "Enter username: ";
    std::cin >> Username;
    std::cout << "Permissions (Ghost, User, Admin, Root): ";
    std::cin >> Permissions;
    UsersM.userSetPermissions(Username, Permissions);
};