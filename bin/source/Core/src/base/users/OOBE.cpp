#include <iostream>
#include "Core/base/print.h"
#include "Core/base/users/user_manager.h"

void OOBE() {
    userManager UM;
    UM.checkOOBE_Passed();
    if (UM.getOOBE_Passed()) return;
    std::string firstUsername;
    print(print::colors::aqua, "Enter username: ");
    std::cin >> firstUsername;
    UM.system_addUser(firstUsername);
}