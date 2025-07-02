#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Core/users/user_manager.h"
#include "Core/settings.h"
#include "Core/print/print.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/experimental/event_manager.h"
#include "Core/utils/other_util.h"

#include "Core/users/user.h"
#include "Core/other/variables.h"

core::UserManager UM;
core::UserManager* core::userManager() {
	return &UM;
}


void core::UserManager::userLogic() {
	#ifndef NRC_WEB
	std::string usernameTemp; std::vector<std::string> temp;
	print("Enter username: ", PrintColors::aqua);
	while (!(std::cin >> std::ws)) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
	std::getline(std::cin, usernameTemp);

	for (auto& user : users) temp.push_back(user->getUsername());
	if (std::find(temp.begin(), temp.end(), usernameTemp) != temp.end())
		userLogin(usernameTemp);
	else { 
		print("User not found\n", PrintColors::red);
		userLogic();
	}
	#endif
}

#ifndef NRC_WEB
void core::UserManager::userLogin(const std::string& username) {
#else
void core::UserManager::userLogin(const std::string& username, const std::string& password) {
#endif
	if (havePassword(username)) {
		#ifndef NRC_WEB
		bool passwordNotPassed = true;
		while (passwordNotPassed) {
			std::string password;
			print("Enter password: ", PrintColors::aqua);
			while (!(std::cin >> std::ws)) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			std::getline(std::cin, password);

			if (!users[userVectorPos(username)]->truePassword(password))
				print("Wrong password!\n", PrintColors::red);
			else {
				passwordNotPassed = false;
				currentUser = username;
				userIsLogined = true;
			}
		}
		#else
		if (!users[userVectorPos(username)]->truePassword(password)) {
			print("Wrong password!\n", PrintColors::red);
		}
		else {
			currentUser = username;
			userIsLogined = true;
		}
		#endif
	}
	else {
		currentUser = username;
		userIsLogined = true;
	}
}

void core::UserManager::addUserFromData(const std::string& username, const std::string& displayName, const core::UserPermissions& permissions, const std::string& language, const std::string& password) {
	users.push_back(new User(username, permissions, language, password));
	users[userVectorPos(username)]->editDisplayName(displayName);

	core::structDataEvents::UserAddEvent eventData = {
		username,
		users[userVectorPos(username)]->displayName,
		users[userVectorPos(username)]->permissions,
		static_cast<size_t>(userVectorPos(username))
	};

	core::EventManager::eventsStart(USER_ADD_EVENT, eventData);
	int code;
	core::pseudoFS()->getFolderData("./home/" + username, 0, code);
	if (code == core::PseudoFSCodes::NOT_FOUND) {
		core::pseudoFS()->createFolder("./home/" + username, 0);
		core::pseudoFS()->setFolderAtt("./home/" + username, 0, "system", true);
	}
	core::pseudoFS()->getNRFS()->saveData();
	//saveUserData(username);
}

core::UserManager::UserManager() {}
bool core::UserManager::userIsYou(const std::string& username) { return username == currentUser; }
bool core::UserManager::userExist(const std::string& username) {
	for (User* user : users) {
		if (user->getUsername() == username) return true;
	}
	return false;
}
bool core::UserManager::havePassword(const std::string& username) { return users[userVectorPos(username)]->havePassword(); }
bool core::UserManager::userHaveAdminPermissions(const std::string& username) { return users[userVectorPos(username)]->getPermissions() >= UserPermissions::ADMIN; }

bool core::UserManager::permissionsHighCurrentUser(const std::string& username) { 
	return users[userVectorPos(currentUser)]->getPermissions() < users[userVectorPos(username)]->getPermissions();
}

bool core::UserManager::getUserIsLogined() { return userIsLogined; }
bool core::UserManager::getOOBEPassed() { return OOBEPassed; }

int core::UserManager::userVectorPos(const std::string& username) {
	int realUsersSize = users.size() - (users.size() == 1 ? 0 : 1);
	for (int i = 0; i <= realUsersSize; i++) {
		if (users[i]->getUsername() == username) return i;
	}
	return -1;
}

core::User &core::UserManager::currentUserData() {
	return *users[userVectorPos(currentUser)];
}
core::User &core::UserManager::getUser(const std::string& username) {
	return *users[userVectorPos(username)];
}
const std::string core::UserManager::yourUsername() { return currentUser; }

std::map<std::string, std::string> core::UserManager::getUserMap() {
	std::map<std::string, std::string> temp;

	for (User* user : users)
		temp[user->getUsername()] = user->getDisplayName();
	return temp;
}

std::vector<core::VariableData> core::UserManager::getLocalVarsMap(const std::string& username) {
	if (userExist(username))
		return users[userVectorPos(username)]->getAllVars();
	return {};
}

std::map<std::string, std::string> core::UserManager::getLanguageMap() {
	std::map<std::string, std::string> temp;

	for (User* user : users)
		temp[user->getUsername()] = user->getLanguage();
	return temp;
}

std::map<std::string, core::UserPermissions> core::UserManager::getPermissionsMap() {
	std::map<std::string, core::UserPermissions> temp;

	for (User* user : users)
		temp[user->getUsername()] = user->getPermissions();
	return temp;
}

void core::UserManager::checkOOBE() {
	if (std::filesystem::exists(mainDataFilePath)) {
		nlohmann::json data = nlohmann::json::parse(core::other_util::getFileContent(mainDataFilePath));
		OOBEPassed = data.at("OOBE_Passed").get<bool>();
	}
}

void core::UserManager::systemAddUser(const std::string& username) {
	users.push_back(new User(username, UserPermissions::ADMIN));
	currentUser = username;
	userIsLogined = true;

	core::structDataEvents::UserAddEvent eventData = {
		username,
		users[userVectorPos(username)]->getDisplayName(),
		users[userVectorPos(username)]->getPermissions(),
		static_cast<size_t>(userVectorPos(username))
	};

	core::EventManager::eventsStart(USER_ADD_EVENT, eventData);

	int code;
	core::pseudoFS()->getFolderData("./home/" + username, 0, code);
	if (code == core::PseudoFSCodes::NOT_FOUND) {
		core::pseudoFS()->createFolder("./home/" + username, 0);
		core::pseudoFS()->setFolderAtt("./home/" + username, 0, "system", true);
	}
	core::pseudoFS()->getNRFS()->saveData();
	saveUserData(username);

	nlohmann::json data = nlohmann::json::parse(core::other_util::getFileContent(mainDataFilePath));
	data["OOBE_Passed"] = true;
	std::ofstream file(mainDataFilePath, std::ios::out);
	file << data.dump(2);
	file.close();

	checkOOBE();
}

void core::UserManager::addUser(const std::string& username, const core::UserPermissions& permissions) {
	if (!userExist(username) && userHaveAdminPermissions(currentUser)
	&& (users.size() < static_cast<size_t>(maxUsers))) 
	{
		users.push_back(new User(username, permissions));

		core::structDataEvents::UserAddEvent eventData = {
			username,
			users[userVectorPos(username)]->getDisplayName(),
			users[userVectorPos(username)]->getPermissions(),
			static_cast<size_t>(userVectorPos(username))
		};

		core::EventManager::eventsStart(USER_ADD_EVENT, eventData);
		int code;
		core::pseudoFS()->getFolderData("./home/" + username, 0, code);
		if (code == core::PseudoFSCodes::NOT_FOUND) {
			core::pseudoFS()->createFolder("./home/" + username, 0);
			core::pseudoFS()->setFolderAtt("./home/" + username, 0, "system", true);
		}
		core::pseudoFS()->getNRFS()->saveData();
		saveUserData(username);
	}
	else print("Failed to create a user\n", PrintColors::red);
}

void core::UserManager::deleteUser(const std::string& username) {
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username)) 
	{
		auto iter = users.begin();
		for (User* user : users) {
			if (user->getUsername() == username) {
				core::structDataEvents::UserDeleteEvent eventData = {
					username,
					users[userVectorPos(username)]->getDisplayName(),
					users[userVectorPos(username)]->getPermissions()
				};

				core::EventManager::eventsStart(USER_DELETE_EVENT, eventData);

				users.erase(iter);
				int code;
				core::pseudoFS()->getFolderData("./home/" + username, 0, code);
				if (code == core::PseudoFSCodes::OK) core::pseudoFS()->deleteFolder("./home/" + username, 0);
				core::pseudoFS()->getNRFS()->saveData();
				break;
			}
			iter++;
		}
		if (std::filesystem::exists(usersPath + username + ".json"))
			std::filesystem::remove(usersPath + username + ".json");
	}
	else print("This user could not be deleted\n", PrintColors::red);
}

void core::UserManager::renameUser(const std::string& username, const std::string& newUsername) {
	if (userExist(username) && userHaveAdminPermissions(currentUser) && 
	!permissionsHighCurrentUser(username) && !userIsYou(username)) 
	{
		users[userVectorPos(username)]->editUsername(newUsername);
		std::filesystem::rename(usersPath + username + ".json", usersPath + newUsername + ".json");

		core::structDataEvents::UserChangeEvent eventData = {
			username, newUsername,
			users[userVectorPos(username)]->getDisplayName(), users[userVectorPos(username)]->getDisplayName(),
			users[userVectorPos(username)]->getPermissions(), users[userVectorPos(username)]->getPermissions(),
			static_cast<size_t>(userVectorPos(username))
		};

		core::EventManager::eventsStart(USER_CHANGE_EVENT, eventData);
	}
	else print("This user could not be renamed\n", PrintColors::red);
}

void core::UserManager::changePermissionsUser(const std::string& username, const core::UserPermissions& newPermissions) {
	if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser)) {
		core::UserPermissions past = users[userVectorPos(username)]->getPermissions();
		users[userVectorPos(username)]->editPermissions(newPermissions);

		nlohmann::json data = core::other_util::getFileContent(usersPath + username + ".json");
		data["Permissions"] = static_cast<int>(newPermissions);
		std::ofstream file(usersPath + username + ".json", std::ios::out);
		file << data.dump(2);
		file.close();

		core::structDataEvents::UserChangeEvent eventData = {
			username, username,
			users[userVectorPos(username)]->getDisplayName(), users[userVectorPos(username)]->getDisplayName(),
			past, users[userVectorPos(username)]->getPermissions(),
			static_cast<size_t>(userVectorPos(username))
		};

		core::EventManager::eventsStart(USER_CHANGE_EVENT, eventData);	
	}
	else print("This user failed to change permissions\n", PrintColors::red);
}

void core::UserManager::userLists() {
	readAllUsersData();
	#ifndef NRC_WEB
	std::cout << " - [ Users ] -" << '\n';

	for (auto& user : users)
		std::cout << " - " << user->getUsername() << '\n';
	userLogic();
	#endif
}

void core::UserManager::userLogout() {
	currentUser = ""; userIsLogined = false;
	userLists();
}

void core::UserManager::saveUserData(const std::string& username) {
	int userPos = userVectorPos(username);
	User* who = users.at(userPos);

	nlohmann::json data;
	data["Display Name"] = who->displayName;
	data["Permissions"] = static_cast<int>(who->permissions);
	data["Language"] = who->language;
	data["Password"] = who->password;
	data["Variables"] = {};
	for (auto var : who->getAllVars()) {
		nlohmann::json varData;
		varData["Name"] = var.name;
		varData["Type"] = static_cast<int>(var.type);
		//varData["Returnable"] = var.outputReturn;
		//varData["Permissions"] = static_cast<int>(var.permissionsRun);
		//varData["Username"] = var.username;
		varData["String"] = var.str;

		data["Variables"].push_back(varData);
	}

	std::ofstream file(usersPath + username + ".json", std::ios::out);
	file << data.dump(2);
	file.close();
}

void core::UserManager::readUserData(const std::string& username) {
	int userPos = userVectorPos(username);
	if (userPos == -1) return;
	User* who = users.at(userPos);

	std::string c = core::other_util::getFileContent(usersPath + username + ".json");
	if (c.empty()) return;
	nlohmann::json data = nlohmann::json::parse(c);

	if (who->getDisplayName() != data.at("Display Name").get<std::string>()) {
		who->editDisplayName(data.at("Display Name").get<std::string>());
	}

	if (who->getPermissions() != data.at("Permissions").get<int>()) {
		who->editPermissions(static_cast<core::UserPermissions>(data.at("Permissions").get<int>()));
	}

	if (who->getLanguage() != data.at("Language").get<std::string>()) {
		who->editLanguage(data.at("Language").get<std::string>());
	}

	if (who->havePassword() && who->password != data.at("Password").get<std::string>()) {
		who->editPassword(data.at("Password").get<std::string>());
	}

	try {
		if (data.at("Variables").is_array()) {
			for (auto jsonVar : data.at("Variables")) {
				if (who->varExists(jsonVar.at("Name").get<std::string>()))
					continue;
				std::string name = jsonVar.at("Name").get<std::string>();
				core::VariableType type = static_cast<core::VariableType>(jsonVar.at("Type").get<int>());
				//bool returnable = jsonVar.at("Returnable").get<bool>();
				std::string str = jsonVar.at("String").get<std::string>();
				//std::string username = jsonVar.at("Username").get<std::string>();
				who->addVar(name, type, str);
			}
		}
	}
	catch(std::exception&) {

	}
}

void core::UserManager::readAllUsersData() {
	for (auto& it : std::filesystem::recursive_directory_iterator("./Data/Users")) {
		std::string file = it.path().filename().generic_string();
		std::string filename = file.substr(0, file.length() - 5);
		if (!userExist(filename)) {
			std::string c = core::other_util::getFileContent(it.path().generic_string());
			if (c.empty()) {
				trace(PrintColors::red, "An error occurred while trying to create user '" + filename + "'\n");
				continue;
			}
			nlohmann::json data = nlohmann::json::parse(c);
			std::string displayName = data.at("Display Name").get<std::string>();
			core::UserPermissions permissions = static_cast<core::UserPermissions>(data.at("Permissions").get<int>());
			std::string language = data.at("Language").get<std::string>();
			std::string password = data.at("Password").get<std::string>();

			addUserFromData(
				filename,
				displayName,
				permissions,
				language,
				password
			);

			readUserData(filename);
		}
		else
			readUserData(filename);
	}
}