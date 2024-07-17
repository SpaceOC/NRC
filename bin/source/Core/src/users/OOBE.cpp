#include <iostream>
#include "Core/users/user_manager.h"

void OOBE() {
    userManager UM;
    UM.checkOOBE_Passed();
    if (!UM.getOOBE_Passed()) {
        userManager UM;
        std::string firstUsername;
        std::cout << "Enter new username: ";
        std::cin >> firstUsername;
        UM.system_addUser(firstUsername);
    }
}