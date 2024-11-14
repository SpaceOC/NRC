#include "Core/base/users/user.h"

core::User::User() {}
core::User::User(const std::string& username, const Permissions& permissions, const std::string& language, const std::string& password) {
    if (!userCreated) {
        this->username = this->displayName = username; this->permissions = permissions;
        this->language = language; this->password = password;
        userCreated = true;
        havePasswordV = havePassword();
    }
}

void core::User::addVar(const std::string& name, const std::string& description, const std::function<void()>& function) {
    if (localVariables.count(name)) return;
    try {
		if (name.empty()) throw std::runtime_error("the 'name' argument was empty!");
		localVariables["%" + name + "%"].description = description;
		localVariables["%" + name + "%"].function = function;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void core::User::editUsername(const std::string& newUsername) { this->username = newUsername; }
void core::User::editDisplayName(const std::string& newDisplayName) { this->displayName = newDisplayName; }
void core::User::editLanguage(const std::string& newLanguage) { this->language = newLanguage; }
void core::User::editPassword(const std::string& newPassword) { this->password = newPassword; havePasswordV = havePassword(); }
void core::User::editPermissions(const core::Permissions& newPermissions) { this->permissions = newPermissions; }
void core::User::editVarFunction(const std::string& name, const std::function<void()>& function) {
    if (localVariables.count(name)) localVariables["%" + name + "%"].function = function;
}
void core::User::editVarDescription(const std::string& name, const std::string& description) {
    if (localVariables.count(name)) localVariables["%" + name + "%"].description = description;
}
void core::User::renameVar(const std::string& oldName, const std::string& newName) {
    if (localVariables.count(oldName) && !localVariables.count(newName)) {
        localVariables[newName] = {localVariables[oldName].description, localVariables[oldName].function};
        localVariables.erase(oldName);
    }
}

bool core::User::truePassword(const std::string& password) { return password == this->password; }
bool core::User::havePassword() { return password.size() > 1 || password != ""; }

std::map<std::string, std::string> core::User::getAllVars() {
    if (localVariables.empty()) return {};
	std::map<std::string, std::string> temp;
	for (auto elements : localVariables) temp[elements.first] = elements.second.description;
	return temp;
}

std::string core::User::getVarDescription(std::string name) {
    if (localVariables.empty()) return "";
    else if (!localVariables.count(name)) return "";

    return localVariables.at(name).description;
}

std::string core::User::getUsername() { return this->username; }
std::string core::User::getDisplayName() { return this->displayName; }
std::string core::User::getLanguage() { return this->language; }
std::string core::User::getPassword() { return this->password; } // DANGER!!!!!!!!!!!!
core::Permissions core::User::getPermissions() { return this->permissions; }

void core::User::varFuncStart(const std::string& name) {
    if (localVariables.empty()) return;
    else if (!localVariables.count(name)) return;

    localVariables.at(name).function();
}