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

core::handlerCommands::handlerCommands() {}

bool core::handlerCommands::thisVariable(const std::string& command) const {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

std::vector<std::string> core::handlerCommands::parsing(const std::string& command) const {
	if (!command.empty()) {
		std::vector<std::string> vectorTemp;
		std::string stringTemp;
		bool first = true;
		for (const char& letter : command + ' ') {
			if (isspace(letter)) {
				if (!first)
					vectorTemp.push_back(stringTemp);
				stringTemp = "";
				first = false;
			}
			else
				stringTemp += letter;
		}
		return vectorTemp;
	}
	return {};
}

std::string core::handlerCommands::realCommand(const std::string& badCommand) const {
	if (!badCommand.empty()) {
		std::string stringTemp;
		for (const char& letter : badCommand) {
			if (isspace(letter))
				return stringTemp;
			else
				stringTemp += letter;
		}
	}
	return "";
}

void core::handlerCommands::sendCommand(const std::string& command, const std::vector<std::string>& args) const {
    auto it = commandMap.find(command);
    if (it != commandMap.end()) { it->second.function(); }
    else if (thisVariable(command)) {
        systemVariables SV; SV.sendVariable(command);
    }
    else {
		auto itArgs = commandWithArgsMap.find(realCommand(command));
    	if (itArgs != commandWithArgsMap.end()) {
			if (itArgs->second.maxArgs < args.size() || itArgs->second.maxArgs > args.size())
				std::cout << "The number of arguments from the user does not equal the number of arguments required by the command. The number of arguments required: " << itArgs->second.maxArgs << '\n';
			else
				itArgs->second.function(args);
		}
		else
			std::cout << "Command not found" << '\n';
	}
}

void core::handlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<void()>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 26 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

void core::handlerCommands::addCommand(const std::string& name, const core::CommandBase& data, const std::function<void(std::vector<std::string>)>& function, int args) {
	std::string temp;
	int realArgsSize = data.argsNames.size() - (data.argsNames.size() <= 1 ? 0 : 1);
	int spacesToAdd = std::max(10, 26 - static_cast<int>(name.length() + data.argsNames.data()->length() + 4 + (3 * realArgsSize)));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + data.description;
	commandWithArgsMap[name].maxArgs = args;
	commandWithArgsMap[name].argsNames = data.argsNames;
	commandWithArgsMap[name].function = function;
	commandWithArgsMap[name].description = temp;
}

std::map<std::string, std::string> core::handlerCommands::getCommand(const std::string& name) const {
	if(commandMap.count(name)) return {{name, commandMap[name].description}};
	return {};
}

std::map<std::string, core::CommandBase> core::handlerCommands::getAllCommands() const {
	if (commandMap.empty()) return {};
	std::map<std::string, core::CommandBase> temp;
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