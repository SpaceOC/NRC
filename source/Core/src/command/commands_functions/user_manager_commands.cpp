#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Core/print/print.h"
#include "Core/print/gprint.h"
#include "Core/command/commands.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/users/user_permissions_enum.h"
#include "Core/command/command_structs.h"
#include "Core/utils/other_util.h"

std::string core::commands::CORE_COMMAND_setPassword(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	if (core::userManager()->havePassword(who->getUsername())) {
		if (!who->truePassword(thisObj->args[0])) {
			return gprint("Wrong password!\n", core::PrintColors::red);
		}
	}
	who->editPassword((core::userManager()->havePassword(who->getUsername()) ? thisObj->args[1] : thisObj->args[0]));
	core::userManager()->saveUserData(who->getUsername());
	return "";
}

std::string core::commands::CORE_COMMAND_editDisplayName(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	who->editDisplayName(thisObj->args.at(0));
	core::userManager()->saveUserData(who->getUsername());
	return "";
}

std::string core::commands::CORE_COMMAND_createUser(core::User*, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	std::string message;
	if (core::userManager()->userExist(thisObj->args.at(0))) {
		return core::gprint("Error: This user already exists\n", core::PrintColors::red);
	}
	else if (thisObj->args.at(1).empty()) {
		message += core::gprint("COMMAND WARNING: A user will be created with \"User\" permissions\n", core::PrintColors::yellow);
	}
	else {
		long long int pos = 1;
		for (const char& letter : thisObj->args.at(0)) {
			if (ispunct(letter) || letter == ',' || letter == '.') {
				return core::gprint("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
			}
			pos++;
		}
		if (!core::other_util::stringIsNumbers(thisObj->args.at(1))) {
			return core::gprint("COMMAND ERROR\n", core::PrintColors::red);
		}
	}
	core::userManager()->addUser(thisObj->args.at(0), static_cast<UserPermissions>((!thisObj->args.at(1).empty() ? stoi(thisObj->args.at(1)) : 0)));
	return message;
}

std::string core::commands::CORE_COMMAND_deleteUser(core::User*, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	if (!core::userManager()->userExist(thisObj->args.at(0))) {
		return core::gprint("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
	}
	else {
		long long int pos = 1;
		for (const char& letter : thisObj->args.at(0)) {
			if (ispunct(letter) || letter == ',' || letter == '.') {
				return core::gprint("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
			}
			pos++;
		}
	}

	core::userManager()->deleteUser(thisObj->args.at(0));
	return "";
};

std::string core::commands::CORE_COMMAND_renameUser(core::User*, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	if (!core::userManager()->userExist(thisObj->args.at(0))) {
		return core::gprint("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
	}
	else {
		long long int pos = 1;
		for (const char& letter : thisObj->args.at(0)) {
			if (ispunct(letter) || letter == ',' || letter == '.') {
				return core::gprint("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
			}
			pos++;
		}
		pos = 1;
		for (const char& letter : thisObj->args.at(1)) {
			if (ispunct(letter) || letter == ',' || letter == '.') {
				return core::gprint("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
			}
			pos++;
		}
	}

	core::userManager()->renameUser(thisObj->args.at(0), thisObj->args.at(1));
	return "";
}

std::string core::commands::CORE_COMMAND_setPermissionsUser(core::User*, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	if (!core::userManager()->userExist(thisObj->args.at(0))) {
		return core::gprint("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
	}
	else {
		long long int pos = 1;
		for (const char& letter : thisObj->args.at(0)) {
			if (ispunct(letter) || letter == ',' || letter == '.') {
				return core::gprint("COMMAND ERROR: Invalid character found (position " + std::to_string(pos) + ")\n", core::PrintColors::red);
			}
			pos++;
		}
		if (!core::other_util::stringIsNumbers(thisObj->args.at(1))) {
			return core::gprint("COMMAND ERROR\n", core::PrintColors::red);
		}
	}
	core::userManager()->changePermissionsUser(thisObj->args.at(0), static_cast<UserPermissions>(stoi(thisObj->args[1])));
	return "";
}

std::string core::commands::CORE_COMMAND_whoim(core::User* who, core::CommandObject*) {
	std::string result = core::gprint(core::PrintColors::white, "Username: " + who->getUsername() + "\n",
		"Display Name: " + who->getDisplayName() + "\n",
		"Permissions: " + userPermissionsS(who->getPermissions()) + "\n"
	);
	return result;
}

std::string core::commands::CORE_COMMAND_infoUser(core::User*, core::CommandObject* thisObj) {
	if (!core::userManager()->userExist(thisObj->args.at(0))) {
		return core::gprint("COMMAND ERROR: The user doesn't exist!\n", core::PrintColors::red);
	}

	std::string result = core::gprint(core::PrintColors::white, "Username: " + thisObj->args.at(0) + "\n",
		"Display Name: " + core::userManager()->getUserMap()[thisObj->args.at(0)] + "\n",
		"Permissions: " + userPermissionsS(core::userManager()->getPermissionsMap()[thisObj->args.at(0)]) + "\n"
	);
	return result;
}

std::string core::commands::CORE_COMMAND_allInfoUsers(core::User*, core::CommandObject*) {
	std::string result = "  - [ All Users Info ] -  \n";
	for (auto& user : core::userManager()->getUserMap()) {
		result += " - " + user.first + " | " + user.second + "\n";
		result += "Language: " + core::userManager()->getLanguageMap()[user.first] + "\n";
		result += "Permissions: " + userPermissionsS(core::userManager()->getPermissionsMap()[user.first]) + "\n";
	}
	return result;
}

std::string core::commands::CORE_COMMAND_logout(core::User*, core::CommandObject* thisObj) {
	if (thisObj->returnable)
		return "Not supported\n";

	std::string choice;
	print("Are you sure you want to log out of your current user account? (Y/N): ", core::PrintColors::yellow);
	std::cin >> choice;
	while (true) {
		if (choice == "Y") { core::userManager()->userLogout(); break; }
		else if (choice == "N") { break; }
		else { 
			print("Error. Are you sure you want to log out of the current user account? (Y/N): ", core::PrintColors::red);
			std::cin >> choice;
		}
	}
	return "";
}