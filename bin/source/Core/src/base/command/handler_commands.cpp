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
	if (handlerCommands::commandMap.empty()) {
		addCommand("help", "shows a list of all commands", CORE_COMMAND_help);
		//addCommand("exit", "exit", CORE_COMMAND_exit);
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

bool handlerCommands::thisVariable(std::string command) const {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

commandBase handlerCommands::parsing(std::string& command) const {
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
	return {};
}

void handlerCommands::sendCommand(std::string command) const {
    auto it = commandMap.find(command);
    if (it != commandMap.end()) { it->second.function(); }
    else if (thisVariable(command)) {
        systemVariables SV; SV.sendVariable(command);
    }
    else { std::cout << "Command not found" << '\n'; }
}

void handlerCommands::addCommand(std::string name, std::string description, std::function<void()> function) const {
	std::string temp;
	int spacesToAdd = std::max(14, 24 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

std::map<std::string, std::string> handlerCommands::getCommand(std::string name) const {
	if(commandMap.count(name)) return {{name, commandMap[name].description}};
	return {};
}

std::map<std::string, std::string> handlerCommands::getAllCommands() const {
	if (commandMap.empty()) return {};
	std::map<std::string, std::string> temp;
	for (auto elements : commandMap) { temp[elements.first] = elements.second.description; }
	return temp;
}
