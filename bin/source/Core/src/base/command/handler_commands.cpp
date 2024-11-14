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
#include "Core/base/print.h"
#include "Core/base/utils.h"

core::HandlerCommands::HandlerCommands() {}

bool core::HandlerCommands::thisVariable(const std::string& command) {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

std::vector<core::CommandObject> core::HandlerCommands::parsing(const std::string& raw) {
	if (!raw.empty()) {
		std::vector<core::CommandObject> result;
		std::vector<std::string> temp = core::Utils::split(raw, ' ');
		temp.push_back(commandSeparator);
		std::vector<std::string> anotherTemp;
		std::string goodString = "";
		std::string backString = "";
		bool quoteOpened = false;
		for (const std::string& rawCommandData : temp) {
			if (rawCommandData == commandSeparator) {
				std::string commandName = anotherTemp.at(0);
				anotherTemp.erase(anotherTemp.begin());
				result.push_back({commandName, anotherTemp});
				anotherTemp = {};
			}
			else if (rawCommandData.substr(0, 1) == "\"" && !quoteOpened) {
				goodString += (rawCommandData.substr(0, 1) == "\"" ?
								rawCommandData.substr(1, rawCommandData.length()) :
								rawCommandData)
							+ " ";
				backString = rawCommandData;
				quoteOpened = true;
				if (rawCommandData.back() == '"') {
					anotherTemp.push_back(goodString.substr(0, goodString.length() - 2));
					goodString.erase();
					quoteOpened = false;
					continue;
				}
			}
			else if (rawCommandData.back() == '"' && quoteOpened) {
				goodString += rawCommandData + " ";
				anotherTemp.push_back(goodString.substr(0, goodString.length() - 2));
				goodString.erase();
				quoteOpened = false;
			}
			else if (quoteOpened && rawCommandData != commandSeparator && rawCommandData != backString) {
				goodString += rawCommandData + " ";
				backString = rawCommandData;
			}
			else {
				if (!goodString.empty()) anotherTemp.push_back(goodString); goodString.erase();
				anotherTemp.push_back(rawCommandData);
			}
		}
		if (quoteOpened) {
			core::print("ERROR: One of the arguments was not closed (missing \" at the end of the argument)\n", core::PrintColors::red);
			return {};
		}
		return result;
	}
	return {};
}

void core::HandlerCommands::sendCommand(const core::CommandObject& command) {
    auto it = commandMap.find(command.name);
    if (it != commandMap.end() && command.args.empty()) { it->second.function(); }
    else if (thisVariable(command.name)) {
        SystemVariablesManager SV; SV.sendVariable(command.name);
    }
    else {
		auto itArgs = commandWithArgsMap.find(command.name);
    	if (itArgs != commandWithArgsMap.end()) {
			if (static_cast<size_t>(itArgs->second.maxArgs) < command.args.size())
				std::cout << "Too many arguments! Maximum number of command arguments: " << itArgs->second.maxArgs << '\n';
			else if (static_cast<size_t>(itArgs->second.minArgs) > command.args.size())
				std::cout << "There are too few arguments! At least '" << itArgs->second.minArgs << "' is required" << '\n';
			else
				itArgs->second.function(command.args);
		}
		else
			std::cout << "Command not found" << '\n';
	}
}

void core::HandlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<void()>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

void core::HandlerCommands::addCommand(const std::string& name, const core::CommandDescription& data, const std::function<void(std::vector<std::string>)>& function, int minArgs, int maxArgs) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length() + data.argsNames.data()->length() + (7 * data.argsNames.size())));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + data.description;
	commandWithArgsMap[name].minArgs = minArgs;
	commandWithArgsMap[name].maxArgs = maxArgs;
	commandWithArgsMap[name].argsNames = data.argsNames;
	commandWithArgsMap[name].function = function;
	commandWithArgsMap[name].description = temp;
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