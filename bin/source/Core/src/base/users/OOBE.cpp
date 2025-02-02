#include <iostream>
#include "Core/base/users/OOBE.h"
#include "Core/base/print.h"
#include "Core/base/users/user_manager.h"

void core::OOBE() {
    #ifndef NRC_WEB
    std::string firstUsername;
    print("Enter username: ", PrintColors::aqua);
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::getline(std::cin, firstUsername);
    core::userManager()->systemAddUser(firstUsername);
    #endif
}