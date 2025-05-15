#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "Core/print/print.h"
#include "Core/users/user.h"
#include "Core/users/user_manager.h"
#include "Core/other/variables.h"
#include "Core/command/command_structs.h"
#include "Core/command/command_parser.h"

core::VariablesManager svm;
core::VariablesManager* core::systemVariablesManager() {
	return &svm;
}

void core::VariablesManager::addVar(const std::string& name, const VariableType& type, const core::UserPermissions& permissionsRun, const std::string& f, const std::string& username, bool outputReturn) {
	try {
		VariableData v;
		v.name = name;
		v.type = type;
		v.username = username;
		v.outputReturn = outputReturn;
		v.str = f;
		v.permissionsRun = (!isSystem ? permissionsRun : core::UserPermissions::ROOT);
		if (type == VariableType::COMMAND) {
			auto func = [](VariableData a) -> std::string {
				std::vector<core::CommandObject> tc = core::handlerCommands()->getParser()->parse(a.str);
				std::string output;
				core::User* who = (
					!a.username.empty() && core::userManager()->userExist(a.username) ? 
					&core::userManager()->getUser(a.username) :
					NULL
				);

				for (const CommandObject& command : tc) {
					std::string temp;
					if (!who)
						core::handlerCommands()->sendCommand(a.permissionsRun, command, temp);
					else
						core::handlerCommands()->sendCommand(who, command, temp);
					output += "\n" + temp;
				}
				if (a.outputReturn)
					return output;
				return "";
			};
			v.function = func;
		}
		else if (type == VariableType::JS_CODE) {
			auto func = [](VariableData a) -> std::string {
				core::User* who = (
					!a.username.empty() && core::userManager()->userExist(a.username) ? 
					&core::userManager()->getUser(a.username) :
					NULL
				);
				
				std::string str;
				if (!who)
					core_experimental::runCode(a.str, a.permissionsRun, str);
				else
					core_experimental::runCode(a.str, who, str);
				if (a.outputReturn)
					return str;
				return "";
			};
			v.function = func;
		}
		else if (type == VariableType::NAME) {
			auto func = [](VariableData a) -> std::string {
				return a.str;
			};
			v.function = func;
		}
		this->data.push_back(v);
	}
	catch (const std::exception& e) {
		core::print(e.what(), core::PrintColors::red);
	}
}

void core::VariablesManager::addVar(const std::string& name, const VariableType& type, const core::UserPermissions& permissionsRun, const std::function<std::string(VariableData)>& f, const std::string& username, bool outputReturn) {
	try {
		VariableData v;
		v.name = name;
		v.type = type;
		v.username = username;
		v.outputReturn = outputReturn;
		v.str = "";
		v.permissionsRun = (!isSystem ? permissionsRun : core::UserPermissions::ROOT);
		v.function = f;
		this->data.push_back(v);
	}
	catch (const std::exception& e) {
		print(e.what(), PrintColors::red);
	}
}

core::VariableData core::VariablesManager::getVariable(std::string_view name) {
	for (auto a : data) {
		if (a.name == name) {
			return a;
		}
	}
	return {};
}

bool core::VariablesManager::exists(std::string_view name) {
	for (auto a : data) {
		if (a.name == name) {
			return true;
		}
	}
	return false;
}

void core::VariablesManager::start(std::string_view variableName) {
	for (auto a : data) {
		if (a.name == variableName) {
			a.function(a);
			break;
		}
	}
}

void core::VariablesManager::start(std::string_view variableName, std::string& str) {
	for (auto a : data) {
		if (a.name == variableName) {
			str = a.function(a);
			break;
		}
	}
}

void core::VariablesManager::rename(const std::string& oldName, const std::string& newName) {
	for (auto a : data) {
		if (a.name == oldName) {
			a.name = newName;
			break;
		}
	}
}

std::vector<core::VariableData> core::VariablesManager::getAllVars() {
	if (data.empty()) return {};
	std::vector<core::VariableData> temp;
	for (auto a : data) temp.push_back(a);
	return temp;
}
