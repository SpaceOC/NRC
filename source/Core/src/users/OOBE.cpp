#include <iostream>
#include "Core/users/OOBE.h"
#include "Core/print/print.h"
#include "Core/users/user_manager.h"

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