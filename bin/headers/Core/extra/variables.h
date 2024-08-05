// Variables header
// by SpaceOC
#pragma once
#ifndef NRC_EXTRA_VARIABLES_H_
#define NRC_EXTRA_VARIABLES_H_
#include <iostream>
#include <string>
#include <functional>
#include <map>

struct variableData {
	std::string description;
	std::function<void()> function;
};

class systemVariables {
	private:
		static inline std::map<std::string, variableData> systemVariablesData;
	public:
		virtual std::map<std::string, variableData> getVariable(std::string name) const;
		virtual void sendVariable(std::string variable) const;
		virtual void addSystemVar(std::string name, std::string description = "", std::function<void()> function = []{}) const;
		virtual std::map<std::string, std::string> getAllVars() const;
};

#endif