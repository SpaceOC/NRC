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
#include <string>
#include <functional>
#include <map>
#include "Core/base/print.h"
#include "Core/extra/variables.h"

std::map<std::string, variableData> systemVariables::getVariable(std::string name) const {
	if (!systemVariablesData.count(name)) 
		return {{"NULL", {"NULL", []{}}}};
	return {{name, {systemVariablesData[name].description, systemVariablesData[name].function}}}; 
}

void systemVariables::sendVariable(std::string variable) const {
	auto it = systemVariablesData.find(variable);
	if (it != systemVariablesData.end()) it->second.function();
}

void systemVariables::addSystemVar(std::string name, std::string description, std::function<void()> function) const {
	try {
		if (name.empty()) throw std::runtime_error("the 'name' argument was empty!");
		systemVariablesData["%" + name + "%"].description = description;
		systemVariablesData["%" + name + "%"].function = function;
	}
	catch (const std::exception& e) {
		print(print::colors::red, e.what());
	}
}

std::map<std::string, std::string> systemVariables::getAllVars() const {
	if (systemVariablesData.empty()) return {{"NULL", "NULL"}};
	std::map<std::string, std::string> temp;
	for (auto elements : systemVariablesData) temp[elements.first] = elements.second.description;
	return temp;
}
