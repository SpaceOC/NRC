#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "Core/extra/variables.h"
#include "Core/base/command/commands.h"
#include "Core/base/command/handler_commands.h"

handlerCommands::handlerCommands() {
	if (handlerCommands::commandMap.size() == 0 && commandMapDescription.size() == 0) {
		addCommand("help", "shows a list of all commands", CORE_COMMAND_help);
		//addCommand("cd", "cd" , CORE_COMMAND_cd);
		//addCommand("tree", "shows all files and folders in a tree view" , CORE_COMMAND_tree);
		addCommand("add_user", "creating a new user in the system", CORE_COMMAND_addUser);
		addCommand("delete_user", "deleting a user in the system", CORE_COMMAND_deleteUser);
		addCommand("set_user_permissions", "user permission change", CORE_COMMAND_setPermissionsUser);
		addCommand("all_users_info", "shows all information about all users", CORE_COMMAND_allInfoUsers);
		addCommand("whoim", "shows information about the current user", CORE_COMMAND_infoUser);
		addCommand("rename_user", "renames the user", CORE_COMMAND_renameUser);
		addCommand("core_info", "shows information about the core", CORE_COMMAND_info);
		addCommand("logout", "logging out of the current user account", CORE_COMMAND_logout);
	}
}

bool handlerCommands::systemVariable(std::string command) const {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

std::vector<std::string> handlerCommands::parsing(std::string& command) const {
	/*
	if (!command.empty()) {
		std::vector<char> src(command.begin(), command.end() + ' ');
		std::vector<std::string> vectorTemp;
		std::string temp;
		for(size_t i = 0; i <= src.size() - 1; i++) {
			if (src[i] == ' ') {
				vectorTemp.push_back(temp);
				temp = "";
			}
			else {
				temp += src[i];
			}
		}
		return vectorTemp;
	}
	*/
	return {""};
}

void handlerCommands::sendCommand(std::string command) const {
    auto it = commandMap.find(command);
    if (it != commandMap.end()) { it->second(); }
    else if (systemVariable(command)) {
        systemVariables SV; SV.sendVariable(command);
        SV.sendVariable(command);
    }
    else { std::cout << "Command not found" << std::endl; }
}

void handlerCommands::addCommand(std::string commandName, std::string commandDescription, std::function<void()> commandFunction) const {
	const int maxSpaces = 20;
	std::string Temp;
	int spacesToAdd = std::max(0, maxSpaces - static_cast<int>(commandName.length()));
	Temp += std::string(spacesToAdd, ' ');
	Temp += "\t  " + commandDescription;
	commandMap[commandName] = commandFunction;
	commandMapDescription[commandName] = Temp;
}

void handlerCommands::getAllCommands() const {
	std::vector<std::string> commandVectorName, commandVectorDescription;
	for (auto elements : commandMap) { commandVectorName.push_back(elements.first); }
	for (auto elements : commandMapDescription) { commandVectorDescription.push_back(elements.second); }
	for (size_t i = 0; (commandVectorName.size() - 1) >= i && (commandVectorDescription.size() - 1) >= i; i++) {
		std::cout << commandVectorName[i] + commandVectorDescription[i] << '\n';
	}
}