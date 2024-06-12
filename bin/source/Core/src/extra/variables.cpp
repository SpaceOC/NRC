#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "Core/extra/variables.h"

std::string systemVariables::getVariable(std::string VarName) const {
	std::string Temp = "";
	if (systemVariablesData.size() == 0) {

	}
	else {
		try
		{
			Temp = systemVariablesData[VarName];
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return Temp;
}

void systemVariables::sendVariable(std::string variable) const {
	auto it = systemVariablesFunction.find(variable);
	if (it != systemVariablesFunction.end()) {
		it->second();
	}
}

void systemVariables::addSystemVar(std::string Name, std::string Var, std::function<void()> VarFunction) const {
	try {
		systemVariablesData[Name] = "%" + Var + "%";
		systemVariablesFunction["%" + Var + "%"] = VarFunction;
		allVars();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void systemVariables::allVars() const {
	for (auto elements : systemVariablesData) {
		std::cout << "Variable name ---> " << elements.first << " | Var ---> " << elements.second << std::endl;
	}
}
