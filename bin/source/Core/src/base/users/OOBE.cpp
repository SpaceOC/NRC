#include <iostream>

#if _WIN32
#include <wutf8console.h>
#endif

#include "Core/base/users/OOBE.h"
#include "Core/base/print.h"
#include "Core/base/users/user_manager.h"

void core::OOBE() {
    userManager UM;
    std::string firstUsername;
    print(print::colors::aqua, "Enter username: ");
    #if _WIN32
    wutf8console::cin >> firstUsername;
    #else
    std::cin >> firstUsername;
    #endif
    UM.system_addUser(firstUsername);
}