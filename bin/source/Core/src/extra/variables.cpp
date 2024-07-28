#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "Core/base/print.h"
#include "Core/extra/variables.h"

std::map<std::string, variableData> systemVariables::getVariable(std::string name) const {
	if (!systemVariablesData.count(name)) return {{"NULL", {"NULL", []{}}}};
	return {{name, {systemVariablesData[name].description, systemVariablesData[name].function}}}; 
}

void systemVariables::sendVariable(std::string variable) const {
	auto it = systemVariablesData.find(variable);
	if (it != systemVariablesData.end()) { it->second.function(); }
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
	for (auto elements : systemVariablesData) { temp[elements.first] = elements.second.description; }
	return temp;
}
