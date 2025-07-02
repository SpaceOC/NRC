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
	core::CommandObject* thisObj = new CommandObject(command);
	bool findCustomRule = std::any_of(customRules.begin(), customRules.end(), [&](std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)> func) -> bool {
		return func(command, who, str, err);
	});
	if (findCustomRule) {
		std::cout << str << '\n';
	}
	else if (!err.empty()) {
		core::print(err, core::PrintColors::red);
		core::print();
	}
	else if (it != commandMap.end() && command.args.empty()) {
		std::cout << it->second.function(who, thisObj);
	}
	else {
		auto itArgs = commandWithArgsMap.find(command.name);
		if (itArgs != commandWithArgsMap.end()) {
			if (static_cast<size_t>(itArgs->second.maxArgs) < command.args.size()) {
				std::cout << "Too many arguments! Maximum number of command arguments: " << itArgs->second.maxArgs << '\n';
			}
			else if (static_cast<size_t>(itArgs->second.minArgs) > command.args.size()) {
				std::cout << "There are too few arguments! At least '" << itArgs->second.minArgs << "' is required" << '\n';
			}
			else {
				std::cout << itArgs->second.function(who, thisObj);
			}
		}
		else {
			std::cout << "Command not found" << '\n';
		}
	}
}

void core::HandlerCommands::sendCommand(core::User* who, const core::CommandObject& command, std::string& str) {
	auto it = commandMap.find(command.name);
	std::string ret, err;
	core::CommandObject* thisObj = new CommandObject(command);
	bool findCustomRule = std::any_of(customRules.begin(), customRules.end(), [&](std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)> func) -> bool {
		return func(command, who, ret, err);
	});
	if (findCustomRule)
		str = ret;
	else if (!err.empty())
		str = err;
	else if (it != commandMap.end() && command.args.empty()) {
		str = it->second.function(who, thisObj);
	}
	else {
		auto itArgs = commandWithArgsMap.find(command.name);
		if (itArgs != commandWithArgsMap.end()) {
			if (static_cast<size_t>(itArgs->second.maxArgs) < command.args.size()) {
				str = "Too many arguments! Maximum number of command arguments: " + core::string_util::valueToString(itArgs->second.maxArgs) + "\n";
			}
			else if (static_cast<size_t>(itArgs->second.minArgs) > command.args.size()) {
				str = "There are too few arguments! At least '" + core::string_util::valueToString(itArgs->second.minArgs) + "' is required\n";
			}
			else {
				str = itArgs->second.function(who, thisObj);
			}
		}
		else {
			str = "Command not found";
		}
	}
}

void core::HandlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<std::string(core::User* who, core::CommandObject*)>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += std::string(STRING_TAB) + "  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

void core::HandlerCommands::addCommand(const std::string& name, const core::CommandDescription& data, const std::function<std::string(core::User*, core::CommandObject*)>& function, int minArgs, int maxArgs, const CommandRules& rules) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length() + data.argsNames.data()->length() + (7 * data.argsNames.size())));
	temp += std::string(spacesToAdd, ' ');
	temp += std::string(STRING_TAB) + "  " + data.description;
	commandWithArgsMap[name].minArgs = minArgs;
	commandWithArgsMap[name].maxArgs = maxArgs;
	commandWithArgsMap[name].argsNames = data.argsNames;
	commandWithArgsMap[name].function = function;
	commandWithArgsMap[name].description = temp;
	commandWithArgsMap[name].rules = new CommandRules(rules);
}

void core::HandlerCommands::addCustomRules(const std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>& f) {
	customRules.push_back(f);
}

void core::HandlerCommands::deleteCommand(const std::string& name) {
	if (!commandMap.count(name) && !commandWithArgsMap.count(name)) return;
	if (commandMap.count(name)) commandMap.erase(name);
	else commandWithArgsMap.erase(name);
}

std::map<std::string, core::CommandDescription> core::HandlerCommands::getCommand(const std::string& name) {
	if (commandMap.count(name))
		return {{name, {commandMap[name].description, {}}}};
	else if (commandWithArgsMap.count(name))
		return {{name, {commandWithArgsMap[name].description, commandWithArgsMap[name].argsNames}}};
	return {};
}

std::map<std::string, core::CommandDescription> core::HandlerCommands::getAllCommands() {
	if (commandMap.empty()) return {};
	std::map<std::string, core::CommandDescription> temp;
	for (auto commandData : commandMap) { 
		temp[commandData.first].description = commandData.second.description;
		temp[commandData.first].argsNames = {};
	}
	for (auto commandData : commandWithArgsMap) { 
		temp[commandData.first].description = commandData.second.description;
		temp[commandData.first].argsNames = commandData.second.argsNames;
	}
	return temp;
}