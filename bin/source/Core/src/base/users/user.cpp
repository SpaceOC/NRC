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
#include "Core/base/users/user.h"

bool user::varExist(std::string name) {
    return localVariables.count(name);
}

user::user() {}
user::user(std::string displayName, permissionsEC permissions, std::string language, std::string password) {
    if (!userCreated) {
        this->displayName = displayName; this->permissions = permissions;
        this->language = language; this->password = password;
        userCreated = true;
    }
}

void user::addVar(std::string name, std::string description, std::function<void()> function) {
    if (varExist(name)) return;
    try {
		if (name.empty()) throw std::runtime_error("the 'name' argument was empty!");
		localVariables["%" + name + "%"].description = description;
		localVariables["%" + name + "%"].function = function;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void user::editDisplayName(std::string newDisplayName) { this->displayName = newDisplayName; }
void user::editLanguage(std::string newLanguage) { this->language = newLanguage; }
void user::editPassword(std::string newPassword) { this->password = newPassword; }
void user::editPermissions(permissionsEC newPermissions) { this->permissions = newPermissions; }
void user::editVarFunction(std::string name, std::function<void()> function) {
    if (varExist(name)) localVariables["%" + name + "%"].function = function;
}
void user::editVarDescription(std::string name, std::string description) {
    if (varExist(name)) localVariables["%" + name + "%"].description = description;
}
void user::renameVar(std::string oldName, std::string newName) {
    if (varExist(oldName) && !varExist(newName)) {
        localVariables[newName] = {localVariables[oldName].description, localVariables[oldName].function};
        localVariables.erase(oldName);
    }
}

bool user::truePassword(std::string password) { return password == this->password; }
bool user::havePassword() { return password.size() > 1 || password != ""; }

std::map<std::string, std::string> user::getAllVars() {
    if (localVariables.empty()) return {{"NULL", "NULL"}};
	std::map<std::string, std::string> temp;
	for (auto elements : localVariables) temp[elements.first] = elements.second.description;
	return temp;
}
std::string user::getVarName() { return "NOTHING"; }
std::string user::getVarDescription() { return "NOTHING"; }
std::string user::getDisplayName() { return this->displayName; }
std::string user::getLanguage() { return this->language; }
permissionsEC user::getPermissions() { return this->permissions; }

/*
std::function<void()> user::varFuncStart(std::string name) {
    if(varExist(name)) localVariables[name].function();
}
*/