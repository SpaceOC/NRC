#include "Core/base/users/user.h"

bool core::user::varExist(const std::string& name) {
    return localVariables.count(name);
}

core::user::user() {}
core::user::user(const std::string& username, const permissionsEC& permissions, const std::string& language, const std::string& password) {
    if (!userCreated) {
        this->username = this->displayName = username; this->permissions = permissions;
        this->language = language; this->password = password;
        userCreated = true;
    }
}

void core::user::addVar(const std::string& name, const std::string& description, const std::function<void()>& function) {
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

void core::user::editUsername(const std::string& newUsername) { this->username = newUsername; }
void core::user::editDisplayName(const std::string& newDisplayName) { this->displayName = newDisplayName; }
void core::user::editLanguage(const std::string& newLanguage) { this->language = newLanguage; }
void core::user::editPassword(const std::string& newPassword) { this->password = newPassword; }
void core::user::editPermissions(const core::permissionsEC& newPermissions) { this->permissions = newPermissions; }
void core::user::editVarFunction(const std::string& name, const std::function<void()>& function) {
    if (varExist(name)) localVariables["%" + name + "%"].function = function;
}
void core::user::editVarDescription(const std::string& name, const std::string& description) {
    if (varExist(name)) localVariables["%" + name + "%"].description = description;
}
void core::user::renameVar(const std::string& oldName, const std::string& newName) {
    if (varExist(oldName) && !varExist(newName)) {
        localVariables[newName] = {localVariables[oldName].description, localVariables[oldName].function};
        localVariables.erase(oldName);
    }
}

bool core::user::truePassword(const std::string& password) { return password == this->password; }
bool core::user::havePassword() { return password.size() > 1 || password != ""; }

std::map<std::string, std::string> core::user::getAllVars() {
    if (localVariables.empty()) return {};
	std::map<std::string, std::string> temp;
	for (auto elements : localVariables) temp[elements.first] = elements.second.description;
	return temp;
}
std::string core::user::getVarName() { return ""; }
std::string core::user::getVarDescription() { return ""; }
std::string core::user::getUsername() { return this->username; }
std::string core::user::getDisplayName() { return this->displayName; }
std::string core::user::getLanguage() { return this->language; }
std::string core::user::getPassword() { return this->password; } // DANGER!!!!!!!!!!!!
core::permissionsEC core::user::getPermissions() { return this->permissions; }

/*
std::function<void()> user::varFuncStart(std::string name) {
    if(varExist(name)) localVariables[name].function();
}
*/