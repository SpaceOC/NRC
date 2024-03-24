#pragma once

#include "user_manager.cpp"

void OOBE() {
    userManager UM;
    std::string firstUsername;
    std::cout << "Enter new username: ";
    std::cin >> firstUsername;
    UM.system_addUser(firstUsername);
}