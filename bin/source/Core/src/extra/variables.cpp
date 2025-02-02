#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "Core/base/print.h"
#include "Core/base/users/user.h"
#include "Core/base/users/user_manager.h"
#include "Core/extra/variables.h"

core::VariablesManager svm;
core::VariablesManager* core::systemVariablesManager() {
	return &svm;
}

void core::VariablesManager::addVar(std::string name, VariableType type, core::Permissions permissionsRun, const std::string& f, const std::string& username, bool outputReturn) {
    try {
        VariableData v;
        v.name = name;
        v.type = type;
        v.username = username;
        v.outputReturn = outputReturn;
        v.commandOrCode = f;
        v.permissionsRun = (!isSystem ? permissionsRun : core::Permissions::root);
        if (type == VariableType::COMMAND) {
            auto func = [](VariableData a) -> std::string {
                std::vector<core::CommandObject> tc = core::handlerCommands()->parsing(a.commandOrCode);
                std::string output;
				core::User* who = (
					!a.username.empty() && core::userManager()->userExist(a.username) ? 
					&core::userManager()->getUser(a.username) :
					NULL
				);

                for (auto command : tc) {
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
                	core_experimental::runCode(a.commandOrCode, a.permissionsRun, str);
				else
					core_experimental::runCode(a.commandOrCode, who, str);
				if (a.outputReturn)
                    return str;
                return "";
        	};
            v.function = func;
        }
        this->data.push_back(v);
    }
    catch (const std::exception& e) {
        core::print(e.what(), core::PrintColors::red);
	}
}

void core::VariablesManager::addVar(std::string name, VariableType type, core::Permissions permissionsRun, const std::function<std::string(VariableData)>& f, const std::string& username, bool outputReturn) {
    try {
        VariableData v;
        v.name = name;
        v.type = type;
        v.username = username;
        v.outputReturn = outputReturn;
        v.commandOrCode = "";
        v.permissionsRun = (!isSystem ? permissionsRun : core::Permissions::root);
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
