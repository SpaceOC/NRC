#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Core/data/data_manager.h"
#include "Core/data/file_manager.h"
#include "Core/users/user_manager.h"


void userManager::userLists() {
	readAllUsersData();
	std::cout << " [ Users ]" << std::endl;
	for (auto& user : users) {
		std::cout << user.first << std::endl;
	}
	userLogic();
}

void userManager::userLogic() {
	std::string usernameTemp; std::vector<std::string> temp;
	std::cout << "Enter username: ";
	std::cin >> usernameTemp;
	for (auto& user : users) { temp.push_back(user.first); }
	if (std::find(temp.begin(), temp.end(), usernameTemp) != temp.end()) { userLogin(usernameTemp); }
	else { std::cout << "User not found" << std::endl; userLogic(); }
}

void userManager::userLogin(std::string username) {
	currentUser = username; userIsLogined = true;
}

void userManager::system_addUser(std::string username, std::string permissions, std::string language) {
	dataManager DM;
	users[username] = username;
	usersPermissions[username] = permissions;
	usersLanguages[username] = language;
	userIsLogined = true;
	saveUserData(username);
}

void userManager::system_readUserData(std::string username) {
	dataManager DM;
	system_addUser(username, DM.readAllData(usersFilesPath + username + ".json")["Permissions"], DM.readAllData(usersFilesPath + username + ".json")["Language"]);
}

userManager::userManager() {
	checkOOBE_Passed();
}

bool userManager::userLangExist(std::string username) {
	return usersLanguages.count(username);
}

bool userManager::userIsYou(std::string username) {
	return username == currentUser;
}

bool userManager::userExist(std::string username) {
	return users.count(username);
}

bool userManager::havePassword(std::string username) {
	return usersPasswords.count(username);
}

bool userManager::userHaveAdminPermissions(std::string username) {
	return usersPermissions[username] == "Admin" || usersPermissions[username] == "Root";
}

bool userManager::permissionsCheck(std::string permissions) {
	return permissions == "Ghost" || permissions == "User" || permissions == "Admin" || permissions == "Root";
}

// Права текущего пользователя ниже другого
bool userManager::permissionsHighCurrentUser(std::string username) const {
	return ((usersPermissions[currentUser] == "Ghost" || usersPermissions[currentUser] == "User") && (username == "Root" || username == "Admin")) || (usersPermissions[currentUser] == "Admin" && username == "Root");
}

bool userManager::getOOBE_Passed() {
	return OOBE_Passed;
}

size_t userManager::numberofUsers() {
	return users.size();
}

std::string userManager::YourUsername() {
	return currentUser;
}

std::map<std::string, std::string> userManager::getUserMap() {
	std::map<std::string, std::string> temp;
	for (auto& element : users) {
		temp[element.first] = element.second;
	}
	return temp;
}

std::map<std::string, std::string> userManager::getLanguageMap() {
	std::map<std::string, std::string> temp;
	for (auto& element : usersLanguages) {
		temp[element.first] = element.second;
	}
	return temp;
}

std::map<std::string, std::string> userManager::getPermissionsMap() {
	std::map<std::string, std::string> temp;
	for (auto& element : usersPermissions) {
		temp[element.first] = element.second;
	}
	return temp;
}

void userManager::checkOOBE_Passed() {
	dataManager DM;
	fileManager FM;
	if (FM.fileExist(mainDataFilePath)) {
		if (DM.readAllData(mainDataFilePath)["OOBE_Passed"] == "1") { OOBE_Passed = true; }
		else { OOBE_Passed = false; }
	}
}

std::vector<std::string> userManager::getUserInfo(std::string username) {
	std::vector<std::string> temp;
	temp.push_back(username);
	temp.push_back(usersPermissions[username]);
	return temp;
}

void userManager::system_addUser(std::string username) {
	dataManager DM;
	users[username] = username;
	usersPermissions[username] = "Root";
	usersLanguages[username] = "ENG";
	currentUser = username;
	userIsLogined = true;
	DM.createData(usersListFilePath, username, "");
	saveUserData(username);
	DM.changeData(mainDataFilePath, "OOBE_Passed", "1", false);
	checkOOBE_Passed();
}

void userManager::addUser(std::string username, std::string permissions) {
	dataManager DM;
	if (!userExist(username) && permissionsCheck(permissions) && userHaveAdminPermissions(currentUser) && (numberofUsers() < maxUsers)) {
		users[username] = username;
		usersPermissions[username] = permissions;
		usersLanguages[username] = "ENG";
		if (!DM.keyExist(usersListFilePath, username)) { DM.addData(usersListFilePath, username, ""); }
		saveUserData(username);
	}
	else {
		std::cout << "Failed to create a user" << std::endl;
	}
}

void userManager::deleteUser(std::string username) {
	try {
		fileManager FM;
		dataManager DM;
		if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser) && !userIsYou(username)) {
			users.erase(username);
			usersPermissions.erase(username);
			if (havePassword(username)) { usersPasswords.erase(username); }
			FM.deleteFile(usersFilesPath + username + ".json");
			DM.deleteData(usersListFilePath, username);
		}
		else {
			std::cout << "This user could not be deleted" << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}

}

void userManager::renameUser(std::string username, std::string new_username) {
	fileManager FM; dataManager DM;
	if (userExist(username) && userHaveAdminPermissions(currentUser) && !permissionsHighCurrentUser(username) && !userIsYou(username)) {
		users.erase(username);
		users[new_username] = new_username;
		usersPermissions[new_username] = usersPermissions[username];
		usersPermissions.erase(username);
		if (havePassword(username)) {
			usersPasswords[new_username] = usersPasswords[username];
			usersPasswords.erase(username);
		}
		FM.renameFile(usersFilesPath + username + ".json", usersFilesPath + new_username + ".json");
		DM.changeData(usersListFilePath, username, new_username, true);
		DM.changeData(usersFilesPath + new_username + ".json", "Username", new_username, false);
	}
	else {
		std::cout << "This user could not be renamed" << std::endl;
	}
}

void userManager::changePermissionsUser(std::string username, std::string newPermissions) {
	dataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser) && permissionsCheck(newPermissions)) {
		usersPermissions[username] = newPermissions;
		DM.changeData(usersFilesPath + username + ".json", "Permissions", newPermissions, false);
	}
	else {
		std::cout << "This user failed to change permissions" << std::endl;
	}
}

void userManager::userListManagerStart() {
	userLists();
}

void userManager::userLogout() {
	currentUser = ""; userIsLogined = false;
	userLists();
}

void userManager::saveUserData(std::string username) const {
	dataManager DM; fileManager FM;
	std::vector<std::string> values = { username, usersPermissions[username], usersLanguages[username], "" };
	FM.createFile(usersFilesPath + username + ".json");
	DM.createData(usersFilesPath + username + ".json", keys, values);
}

void userManager::readUserData(std::string username) const {
	dataManager DM;
	usersLanguages[username] = DM.readAllData("Data/Users/" + username + ".json")["Language"];
}

void userManager::readAllUsersData() {
	dataManager DM;
	for (auto& user : DM.readAllData(usersListFilePath)) {
		system_readUserData(user.first);
	}
}
