#pragma once
#ifndef VARIABLES
#define VARIABLES
#include <iostream>
#include <string>
#include <functional>
#include <map>

class systemVariables {
private:
	static inline std::map<std::string, std::string> systemVariablesData, localVariablesData;
	static inline std::map<std::string, std::function<void()>> systemVariablesFunction, localVariablesFunction;
public:
	virtual std::string getVariable(std::string VarName) const;

	virtual void sendVariable(std::string variable) const;

	virtual void addSystemVar(std::string Name, std::string Var, std::function<void()> VarFunction) const;

	virtual void allVars() const;
};
#endif