#include <iostream>
#include "Core/base/print.h"
#include "Core/base/data/data_manager.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/users/user_manager.h"

void core::userManager::userLogic() {
	std::string usernameTemp; std::vector<std::string> temp;
	print(print::colors::aqua, "Enter username: ");
	std::cin >> usernameTemp;

	for (auto& user : users) temp.push_back(user.getUsername());
	if (std::find(temp.begin(), temp.end(), usernameTemp) != temp.end())
		userLogin(usernameTemp);
	else { 
		print(print::colors::red, "User not found\n");
		userLogic();
	}
}

void core::userManager::userLogin(const std::string& username) {
	currentUser = username;
	userIsLogined = true;
}

void core::userManager::addUserFromData(const std::string& username, const permissionsEC& permissions, const std::string& language, const std::string& password) {
	users.push_back(user(username, permissions, language));
	saveUserData(username);
}

core::userManager::userManager() { checkOOBE_Passed(); }
bool core::userManager::userIsYou(const std::string& username) { return username == currentUser; }
bool core::userManager::userExist(const std::string& username) {
	for (user& user : users) {
		if (user.getUsername() == username) return true;
	}
	return false;
}
bool core::userManager::havePassword(const std::string& username) { return users[userVectorPos(username)].havePassword(); }
bool core::userManager::userHaveAdminPermissions(const std::string& username) { return users[userVectorPos(username)].getPermissions() == permissionsEC::admin; }

bool core::userManager::permissionsCheck(const permissionsEC& permissions) {
	for (int i = static_cast<int>(permissionsEC::ghost); i <= static_cast<int>(permissionsEC::admin); i++)
		if (permissions == static_cast<permissionsEC>(i)) 
			return true;
	return false;
}

// Права текущего пользователя ниже другого

bool core::userManager::permissionsHighCurrentUser(const std::string& username) { 
	return users[userVectorPos(currentUser)].getPermissions() < users[userVectorPos(username)].getPermissions();
}

bool core::userManager::getUserIsLogined() { return userIsLogined; }
bool core::userManager::getOOBE_Passed() { return OOBE_Passed; }

int core::userManager::userVectorPos(const std::string& username) {
	int realUsersSize = users.size() - (users.size() == 1 ? 0 : 1);
	for (int i = 0; i <= realUsersSize; i++) {
		if (users[i].getUsername() == username) return i;
	}
	return -1;
}

const std::string core::userManager::yourUsername() { return currentUser; }

std::map<std::string, std::string> core::userManager::getUserMap() {
	std::map<std::string, std::string> temp;

	for (user& user : users)
		temp[user.getUsername()] = user.getDisplayName();
	return temp;
}

std::map<std::string, std::string> core::userManager::getLocalVarsMap(const std::string& username) {
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username))
		return users[userVectorPos(username)].getAllVars();
	else if (userExist(username) && userIsYou(username)) 
		return users[userVectorPos(username)].getAllVars();
	return {};
}

std::map<std::string, std::string> core::userManager::getLanguageMap() {
	std::map<std::string, std::string> temp;

	for (user& user : users)
		temp[user.getUsername()] = user.getLanguage();
	return temp;
}

std::map<std::string, core::permissionsEC> core::userManager::getPermissionsMap() {
	std::map<std::string, permissionsEC> temp;

	for (user& user : users)
		temp[user.getUsername()] = user.getPermissions();
	return temp;
}

void core::userManager::checkOOBE_Passed() {
	dataManager DM;
	fileManager FM;

	if (FM.fileExist(mainDataFilePath))
		OOBE_Passed = DM.readAllData(mainDataFilePath)["OOBE_Passed"] == "1";
}

void core::userManager::system_addUser(const std::string& username) {
	dataManager DM;
	users.push_back(user(username, permissionsEC::admin));
	currentUser = username;
	userIsLogined = true;
	DM.createVectorData(usersListFilePath, {username});
	saveUserData(username);
	DM.changeData(mainDataFilePath, "OOBE_Passed", "1");
	checkOOBE_Passed();
}

void core::userManager::addUser(const std::string& username, const permissionsEC& permissions) {
	dataManager DM;
	if (!userExist(username) && permissionsCheck(permissions) && 
	userHaveAdminPermissions(currentUser) && (users.size() < maxUsers)) 
	{
		users.push_back(user(username, permissions));
		if (!DM.valueExist(usersListFilePath, username)) { DM.addVectorData(usersListFilePath, username); }
		saveUserData(username);
	}
	else print(print::colors::red, "Failed to create a user\n");
}

void core::userManager::deleteUser(const std::string& username) {
	fileManager FM;
	dataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username)) 
	{
		auto iter = users.begin();
		for (user& user : users) {
			if (user.getUsername() == username) {
				users.erase(iter);
				break;
			}
			iter++;
		}
		FM.deleteFile(usersFilesPath + username + ".json");
		DM.deleteVectorData(usersListFilePath, username);
	}
	else print(print::colors::red, "This user could not be deleted\n");
}

void core::userManager::renameUser(const std::string& username, const std::string& newUsername) {
	fileManager FM; dataManager DM;
	if (userExist(username) && userHaveAdminPermissions(currentUser) && 
	!permissionsHighCurrentUser(username) && !userIsYou(username)) 
	{
		
		user userTemp = users[userVectorPos(username)];
		auto iter = users.begin();
		for (user& user : users) {
			if (user.getUsername() == username) {
				users.erase(iter);
				break;
			}
			iter++;
		}
		userTemp.editUsername(newUsername);
		users.push_back(userTemp);
		FM.renameFile(usersFilesPath + username + ".json", usersFilesPath + newUsername + ".json");
		DM.changeVectorData(usersListFilePath, username, newUsername);
		DM.changeData(usersFilesPath + newUsername + ".json", "Username", newUsername);
	}
	else print(print::colors::red, "This user could not be renamed\n");
}

void core::userManager::changePermissionsUser(const std::string& username, const permissionsEC& newPermissions) {
	dataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser)) {
		users[userVectorPos(username)].editPermissions(newPermissions);
		DM.changeData(usersFilesPath + username + ".json", "Permissions", std::to_string(static_cast<int>(newPermissions)));
	}
	else print(print::colors::red, "This user failed to change permissions\n");
}

void core::userManager::userLists() {
	readAllUsersData();
	std::cout << " - [ Users ] -" << '\n';

	for (auto& user : users)
		std::cout << " - " << user.getUsername() << '\n';
	userLogic();
}

void core::userManager::userLogout() {
	currentUser = ""; userIsLogined = false;
	userLists();
}

void core::userManager::saveUserData(const std::string& username) {
	dataManager DM; fileManager FM;
	int userPos = userVectorPos(username);
	std::vector<std::string> values = { username, users[userPos].getDisplayName(), std::to_string(static_cast<int>(users[userPos].getPermissions())), users[userPos].getLanguage(), "" };
	FM.createFile(usersFilesPath + username + ".json");
	DM.createData(usersFilesPath + username + ".json", keys, values);
}

void core::userManager::readUserData(const std::string& username) {
	dataManager DM;
	int userPos = userVectorPos(username);

	if (users[userPos].getLanguage() != 
		DM.getValue("Data/Users/" + username + ".json","Language"))
			users[userPos].editLanguage(DM.getValue("Data/Users/" + username + ".json","Language"));
	
	if (users[userPos].getDisplayName() !=
		DM.getValue("Data/Users/" + username + ".json","Diplay Name"))
			users[userPos].editDisplayName(DM.getValue("Data/Users/" + username + ".json", "Display Name"));
	
	if (users[userPos].getPermissions() !=
		static_cast<permissionsEC>(stoi(
			DM.getValue("Data/Users/" + username + ".json","Permissions")
			))) 
				users[userPos].editPermissions((static_cast<permissionsEC>(stoi(
					DM.getValue("Data/Users/" + username + ".json", "Permissions")
				))));
	
	if (users[userPos].havePassword() != 
		DM.getValue("Data/Users/" + username + ".json", "Password").empty()) 
			users[userPos].editPassword(DM.getValue(
				"Data/Users/" + username + ".json", "Password"
			));
}

void core::userManager::readAllUsersData() {
	fileManager FM;
	dataManager DM;
	for (const std::string& user : DM.readAllVectorData(usersListFilePath)) {
		if (!userExist(user) && FM.fileExist("Data/Users/" + user + ".json")) 
			addUserFromData(user, static_cast<permissionsEC>(stoi(DM.readAllData(usersFilesPath + user + ".json")["Permissions"])), DM.readAllData(usersFilesPath + user + ".json")["Language"]);
		else if (userExist(user) && FM.fileExist("Data/Users/" + user + ".json")) 
			readUserData(user);
		else 
			print(print::colors::red, "An error occurred while trying to create user '" + user + "' | readAllUsersData()\n");
	}
}