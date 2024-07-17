#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Core/data/data_manager.h"
#include "Core/data/file_manager.h"
#include "Core/users/user_manager.h"


void userManager::userLists() {
	readAllUsersData();
	std::cout << " [ Users ]" << '\n';
	for (auto& user : users) {
		std::cout << user.first << '\n';
	}
	userLogic();
}

void userManager::userLogic() {
	std::string usernameTemp; std::vector<std::string> temp;
	std::cout << "Enter username: ";
	std::cin >> usernameTemp;
	for (auto& user : users) { temp.push_back(user.first); }
	if (std::find(temp.begin(), temp.end(), usernameTemp) != temp.end()) { userLogin(usernameTemp); }
	else { std::cout << "User not found" << '\n'; userLogic(); }
}

void userManager::userLogin(std::string username) {
	currentUser = username;
	userIsLogined = true;
}

void userManager::addUserFromData(std::string username, permissionsEC permissions, std::string language, std::string password) {
	users[username] = user(username, permissions, language);
	userIsLogined = true;
	saveUserData(username);
}

userManager::userManager() { checkOOBE_Passed(); }
bool userManager::userIsYou(std::string username) { return username == currentUser; }
bool userManager::userExist(std::string username) { return users.count(username); }
bool userManager::havePassword(std::string username) { return users[username].havePassword(); }
bool userManager::userHaveAdminPermissions(std::string username) { return users[username].getPermissions() == permissionsEC::admin; }

bool userManager::permissionsCheck(permissionsEC permissions) {
	for(int i = static_cast<int>(permissionsEC::ghost); i <= static_cast<int>(permissionsEC::admin); i++) {
		if (permissions == static_cast<permissionsEC>(i)) return true;
	}
	return false;
}

// Права текущего пользователя ниже другого
bool userManager::permissionsHighCurrentUser(std::string username) const { return users[currentUser].getPermissions() < users[username].getPermissions(); }
bool userManager::getOOBE_Passed() { return OOBE_Passed; }
size_t userManager::numberofUsers() { return users.size(); }
std::string userManager::yourUsername() { return currentUser; }

std::map<std::string, std::string> userManager::getUserMap() {
	std::map<std::string, std::string> temp;
	for (auto& element : users) {
		temp[element.first] = element.second.getDisplayName();
	}
	return temp;
}

std::map<std::string, std::string> userManager::getLanguageMap() {
	std::map<std::string, std::string> temp;
	for (auto& element : users) {
		temp[element.first] = element.second.getLanguage();
	}
	return temp;
}

std::map<std::string, permissionsEC> userManager::getPermissionsMap() {
	std::map<std::string, permissionsEC> temp;
	for (auto& element : users) {
		temp[element.first] = element.second.getPermissions();
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

void userManager::system_addUser(std::string username) {
	dataManager DM;
	users[username] = user(username, permissionsEC::admin);
	currentUser = username;
	userIsLogined = true;
	DM.createVectorData(usersListFilePath, {username});
	saveUserData(username);
	DM.changeData(mainDataFilePath, "OOBE_Passed", "1", false);
	checkOOBE_Passed();
}

void userManager::addUser(std::string username, permissionsEC permissions) {
	dataManager DM;
	if (!userExist(username) && permissionsCheck(permissions) && userHaveAdminPermissions(currentUser) && (numberofUsers() < maxUsers)) {
		users[username] = user(username, permissions);
		if (!DM.valueExist(usersListFilePath, username)) { DM.addVectorData(usersListFilePath, username); }
		saveUserData(username);
	}
	else {
		std::cout << "Failed to create a user" << '\n';
	}
}

void userManager::deleteUser(std::string username) {
	try {
		fileManager FM;
		dataManager DM;
		if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser) && !userIsYou(username)) {
			users.erase(username);
			FM.deleteFile(usersFilesPath + username + ".json");
			DM.deleteVectorData(usersListFilePath, username);
		}
		else {
			std::cout << "This user could not be deleted" << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void userManager::renameUser(std::string username, std::string newUsername) {
	fileManager FM; dataManager DM;
	if (userExist(username) && userHaveAdminPermissions(currentUser) && !permissionsHighCurrentUser(username) && !userIsYou(username)) {
		user userTemp = users[username];
		users.erase(username);
		users[newUsername] = userTemp;
		FM.renameFile(usersFilesPath + username + ".json", usersFilesPath + newUsername + ".json");
		DM.changeVectorData(usersListFilePath, username, newUsername);
		DM.changeData(usersFilesPath + newUsername + ".json", "Username", newUsername, false);
	}
	else {
		std::cout << "This user could not be renamed" << '\n';
	}
}

void userManager::changePermissionsUser(std::string username, permissionsEC newPermissions, std::string password) {
	dataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser)) {
		users[username].editPermissions(newPermissions);
		DM.changeData(usersFilesPath + username + ".json", "Permissions", std::to_string(static_cast<int>(newPermissions)), false);
	}
	else {
		std::cout << "This user failed to change permissions" << '\n';
	}
}

void userManager::userListManagerStart() { userLists(); }

void userManager::userLogout() {
	currentUser = ""; userIsLogined = false;
	userLists();
}

void userManager::saveUserData(std::string username) const {
	dataManager DM; fileManager FM;
	std::vector<std::string> values = { username, users[username].getDisplayName(), std::to_string(static_cast<int>(users[username].getPermissions())), users[username].getLanguage(), "" };
	FM.createFile(usersFilesPath + username + ".json");
	DM.createData(usersFilesPath + username + ".json", keys, values);
}

void userManager::readUserData(std::string username) const {
	dataManager DM;
	users[username].editLanguage(DM.getValue("Data/Users/" + username + ".json","Language"));
	users[username].editDisplayName(DM.getValue("Data/Users/" + username + ".json", "Display Name"));
	users[username].editPermissions((static_cast<permissionsEC>(stoi(DM.getValue("Data/Users/" + username + ".json", "Permissions")))));
	users[username].editPassword(DM.getValue("Data/Users/" + username + ".json", "Password"));
}

void userManager::readAllUsersData() {
	dataManager DM;
	for (auto& user : DM.readAllVectorData(usersListFilePath)) {
		if (!userExist(user)) addUserFromData(user, static_cast<permissionsEC>(stoi(DM.readAllData(usersFilesPath + user + ".json")["Permissions"])), DM.readAllData(usersFilesPath + user + ".json")["Language"]);
	}
}