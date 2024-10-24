#include <iostream>
#include "Core/base/users/OOBE.h"
#include "Core/base/print.h"
#include "Core/base/users/user_manager.h"

void core::OOBE() {
    std::string firstUsername;
    print(colors::aqua, "Enter username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, firstUsername);
    core::UserManager::systemAddUser(firstUsername);
}