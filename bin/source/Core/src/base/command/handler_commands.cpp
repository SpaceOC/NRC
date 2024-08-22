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

core::commandBase core::handlerCommands::parsing(std::string& command) const {
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

void core::handlerCommands::sendCommand(const std::string& command) const {
    auto it = commandMap.find(command);
    if (it != commandMap.end()) { it->second.function(); }
    else if (thisVariable(command)) {
        systemVariables SV; SV.sendVariable(command);
    }
    else { std::cout << "Command not found" << '\n'; }
}

void core::handlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<void()>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 26 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += "\t  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

std::map<std::string, std::string> core::handlerCommands::getCommand(const std::string& name) const {
	if(commandMap.count(name)) return {{name, commandMap[name].description}};
	return {};
}

std::map<std::string, std::string> core::handlerCommands::getAllCommands() const {
	if (commandMap.empty()) return {};
	std::map<std::string, std::string> temp;
	for (auto elements : commandMap) { temp[elements.first] = elements.second.description; }
	return temp;
}