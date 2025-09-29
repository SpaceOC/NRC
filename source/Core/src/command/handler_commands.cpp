#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "Core/command/handler_commands.h"
#include "Core/command/command_structs.h"
#include "Core/other/variables.h"
#include "Core/utils/string_util.h"
#include "Core/print/print.h"
#include "Core/users/user.h"
#include "Core/users/user_manager.h"
#include "Core/users/user_permissions_enum.h"

#ifndef NRC_WEB
#define STRING_TAB		"\t"
#else
#define STRING_TAB		"    "
#endif

core::HandlerCommands HC;
core::HandlerCommands* core::handlerCommands() {
	return &HC;
}

core::HandlerCommands::HandlerCommands() {
	parser = new CommandParser();
}

bool core::HandlerCommands::thisVariable(const std::string& command) {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

void core::HandlerCommands::sendCommand(const core::CommandObject& command) {
	sendCommand(&core::userManager()->currentUserData(), command);
}

void core::HandlerCommands::sendCommand(const core::CommandObject& command, std::string& str) {
	sendCommand(&core::userManager()->currentUserData(), command, str);
}

void core::HandlerCommands::sendCommand(const core::UserPermissions permissions, const core::CommandObject& command) {
	core::User* user = new User("UNKNOWN_TEMP_USER_FOR_RUNNING_COMMANDS_OR_JS_CODE", permissions);
	sendCommand(user, command);
	delete user;
}

void core::HandlerCommands::sendCommand(const core::UserPermissions permissions, const core::CommandObject& command, std::string& str) {
	core::User* user = new User("UNKNOWN_TEMP_USER_FOR_RUNNING_COMMANDS_OR_JS_CODE", permissions);
	sendCommand(user, command, str);
	delete user;
}

void core::HandlerCommands::sendCommand(core::User* who, const core::CommandObject& command) {
	auto it = commandMap.find(command.name);
	std::string str, err;
	bool findCustomRule = false;
	core::CommandObject* thisObj = new CommandObject(command);
	for (auto pair : customRules) {
		if ((findCustomRule = pair.second(command, who, str, err))) {
			break;
		}
	}
	if (findCustomRule) {
		std::cout << str << '\n';
	}
	else if (!err.empty()) {
		core::print(err, core::PrintColors::red);
		core::print();
	}
	else if (it != commandMap.end()) {
		if (std::holds_alternative<SimpleCommand>(it->second)) {
			SimpleCommand simpleCommand = std::get<SimpleCommand>(it->second);
			std::cout << simpleCommand(who, thisObj);
		}
		else {
			ExtentedCommand extentedCommand = std::get<ExtentedCommand>(it->second);
			if (static_cast<size_t>(extentedCommand.maxArgs) < command.args.size()) {
				std::cout << "Too many arguments! Maximum number of command arguments: " << extentedCommand.maxArgs << '\n';
			}
			else if (static_cast<size_t>(extentedCommand.minArgs) > command.args.size()) {
				std::cout << "There are too few arguments! At least '" << extentedCommand.minArgs << "' is required" << '\n';
			}
			else {
				std::cout << extentedCommand.function(who, thisObj);
			}
		}
	}
	else {
			std::cout << "Command not found" << '\n';
	}
}

void core::HandlerCommands::sendCommand(core::User* who, const core::CommandObject& command, std::string& str) {
	auto it = commandMap.find(command.name);
	std::string ret, err;
	bool findCustomRule = false;
	core::CommandObject* thisObj = new CommandObject(command);
	for (auto pair : customRules) {
		if ((findCustomRule = pair.second(command, who, ret, err))) {
			break;
		}
	}
	if (findCustomRule)
		str = ret;
	else if (!err.empty())
		str = err;
	else if (it != commandMap.end()) {
		if (std::holds_alternative<SimpleCommand>(it->second)) {
			SimpleCommand simpleCommand = std::get<SimpleCommand>(it->second);
			str = simpleCommand(who, thisObj);
		}
		else {
			ExtentedCommand extentedCommand = std::get<ExtentedCommand>(it->second);
			if (static_cast<size_t>(extentedCommand.maxArgs) < command.args.size()) {
				str = "Too many arguments! Maximum number of command arguments: " + core::string_util::valueToString(extentedCommand.maxArgs) + "\n";
			}
			else if (static_cast<size_t>(extentedCommand.minArgs) > command.args.size()) {
				str = "There are too few arguments! At least '" + core::string_util::valueToString(extentedCommand.minArgs) + "' is required\n";
			}
			else {
				str = extentedCommand.function(who, thisObj);
			}
		}
	}
	else {
		str = "Command not found";
	}
}

void core::HandlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<std::string(core::User* who, core::CommandObject*)>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += std::string(STRING_TAB) + "  " + description;
	SimpleCommand com = function;
	commandMap[name] = com;
	commandInfo[name].description = temp;
}

void core::HandlerCommands::addCommand(const std::string& name, const core::CommandDescription& data, const std::function<std::string(core::User*, core::CommandObject*)>& function, int minArgs, int maxArgs, const CommandRules& rules) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length() + data.argsNames.data()->length() + (7 * data.argsNames.size())));
	temp += std::string(spacesToAdd, ' ');
	temp += std::string(STRING_TAB) + "  " + data.description;
	ExtentedCommand com = {minArgs, maxArgs, function, new CommandRules(rules)};
	commandMap[name] = com;
	commandInfo[name].description = temp;
	commandInfo[name].argsNames = data.argsNames;
}

void core::HandlerCommands::addCustomRules(const std::string& id, const CustomRulesFunc& f) {
	customRules[id] = f;
}

void core::HandlerCommands::deleteCustomRules(const std::string& id) {
	if (customRules.count(id)) {
		customRules.erase(id);
	}
}

void core::HandlerCommands::deleteCommand(const std::string& name) {
	if (commandMap.count(name)) {
		commandMap.erase(name);
	}
	
	if (commandInfo.count(name)) {
		commandInfo.erase(name);
	}
}

std::map<std::string, core::CommandDescription> core::HandlerCommands::getCommand(const std::string& name) {
	if (commandMap.count(name))
		return {{name, commandInfo[name]}};
	return {};
}

std::map<std::string, core::CommandDescription> core::HandlerCommands::getAllCommands() {
	return commandInfo;
}