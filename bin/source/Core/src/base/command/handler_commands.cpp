/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "Core/extra/variables.h"
#include "Core/base/command/commands.h"
#include "Core/base/command/handler_commands.h"
#include "Core/base/utils.h"

core::handlerCommands::handlerCommands() {}

bool core::handlerCommands::thisVariable(const std::string& command) const {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

std::vector<core::CommandObject> core::handlerCommands::parsing(const std::string& rawCommand) const {
	if (!rawCommand.empty()) {
		std::vector<core::CommandObject> result;
		std::vector<std::string> temp = core::Utils::split(rawCommand, ' ');
		temp.push_back(commandSeparator);
		std::vector<std::string> anotherTemp;
		for (const std::string& rawCommandData : temp) {
			if (rawCommandData == commandSeparator) {
				std::string commandName = anotherTemp.at(0);
				anotherTemp.erase(anotherTemp.begin());
				result.push_back({commandName, anotherTemp});
				anotherTemp = {};
			}
			else
				anotherTemp.push_back(rawCommandData);
		}
		return result;
	}
	return {};
}

void core::handlerCommands::sendCommand(const core::CommandObject& command) const {
    auto it = commandMap.find(command.name);
    if (it != commandMap.end() && command.args.empty()) { it->second.function(); }
    else if (thisVariable(command.name)) {
        systemVariables SV; SV.sendVariable(command.name);
    }
    else {
		auto itArgs = commandWithArgsMap.find(command.name);
    	if (itArgs != commandWithArgsMap.end()) {
			if (itArgs->second.maxArgs < command.args.size())
				std::cout << "Too many arguments! Maximum number of command arguments: " << itArgs->second.maxArgs << '\n';
			else if (itArgs->second.minArgs > command.args.size())
				std::cout << "There are too few arguments! At least '" << itArgs->second.minArgs << "' is required" << '\n';
			else
				itArgs->second.function(command.args);
		}
		else
			std::cout << "Command not found" << '\n';
	}
}

void core::handlerCommands::setCommandSeparator(const std::string& newCommandSeparator) {
	commandSeparator = newCommandSeparator;
}

void core::handlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<void()>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 26 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

void core::handlerCommands::addCommand(const std::string& name, const core::CommandDescription& data, const std::function<void(std::vector<std::string>)>& function, int minArgs, int maxArgs) {
	std::string temp;
	int spacesToAdd = std::max(10, 26 - static_cast<int>(name.length() + data.argsNames.data()->length() + (3 * data.argsNames.size())));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + data.description;
	commandWithArgsMap[name].minArgs = minArgs;
	commandWithArgsMap[name].maxArgs = maxArgs;
	commandWithArgsMap[name].argsNames = data.argsNames;
	commandWithArgsMap[name].function = function;
	commandWithArgsMap[name].description = temp;
}

std::map<std::string, core::CommandDescription> core::handlerCommands::getCommand(const std::string& name) const {
	if (commandMap.count(name))
		return {{name, {commandMap[name].description, {}}}};
	else if (commandWithArgsMap.count(name))
		return {{name, {commandWithArgsMap[name].description, commandWithArgsMap[name].argsNames}}};
	return {};
}

std::map<std::string, core::CommandDescription> core::handlerCommands::getAllCommands() const {
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