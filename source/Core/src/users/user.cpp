#include "Core/users/user.h"
#include "Core/other/variables.h"
#include "Core/print/print.h"

core::User::User() {}
core::User::User(const std::string& username, const core::UserPermissions& permissions, const std::string& language, const std::string& password) {
	if (!userCreated) {
		this->vm = new core::VariablesManager(false);
		this->username = this->displayName = username;
		this->permissions = permissions;
		this->language = language;
		this->password = password;
		userCreated = true;
		havePasswordV = havePassword();
	}
}
core::User::User(core::User& user) {
	this->username = user.username;
	this->displayName = user.displayName;
	if (this->displayName.empty())
		this->displayName = user.username;

	this->password = user.password;
	this->vm = user.vm;
	if (!this->vm)
		this->vm = new core::VariablesManager(false);

	this->permissions = user.permissions;
	this->language = user.language;
	this->userCreated = user.userCreated = true;
	this->havePasswordV = havePassword();
}

void core::User::editUsername(const std::string& newUsername) { this->username = newUsername; }
void core::User::editDisplayName(const std::string& newDisplayName) { this->displayName = newDisplayName; }
void core::User::editLanguage(const std::string& newLanguage) { this->language = newLanguage; }
void core::User::editPassword(const std::string& newPassword) { this->password = newPassword; havePasswordV = havePassword(); }
void core::User::editPermissions(const core::UserPermissions& newPermissions) { this->permissions = newPermissions; }

bool core::User::truePassword(const std::string& password) { return password == this->password; }
bool core::User::havePassword() { return password.size() > 1 || password != ""; }

std::string core::User::getUsername() { return this->username; }
std::string core::User::getDisplayName() { return this->displayName; }
std::string core::User::getLanguage() { return this->language; }
core::UserPermissions core::User::getPermissions() { return this->permissions; }

void core::User::addVar(const std::string& name, const VariableType type, const std::string& c) {
	if (!vm) return;
	vm->addVar(name, type, this->permissions, c, this->username, true);
}

void core::User::addVar(const std::string& name, const VariableType type, const std::function<std::string(VariableData)>& f) {
	if (!vm) return;
	vm->addVar(name, type, this->permissions, f, this->username, true);
}

void core::User::renameVar(const std::string& oldName, const std::string& newName) {
	if (!vm) return;
	vm->rename(oldName, newName);
}

void core::User::varFuncStart(std::string_view name) {
	if (!vm) return;
	std::string str;
	vm->start(name, str);
	core::print(str, core::PrintColors::green);
	core::print();
}

void core::User::varFuncStart(std::string_view name, std::string& str) {
	if (!vm) return;
	vm->start(name, str);
}

bool core::User::varExists(std::string_view name) {
	if (!vm) return false;
	return vm->exists(name);
}

std::vector<core::VariableData> core::User::getAllVars() {
	if (!vm) return {};
	return vm->getAllVars();
}

core::VariableData core::User::getVar(std::string_view name) {
	if (!vm) return {};
	return vm->getVariable(name);
}