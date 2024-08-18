#include <iostream>
#include "Core/base/print.h"
#include "Core/base/data/data_manager.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/users/user_manager.h"


void userManager::userLogic() {
	std::string usernameTemp; std::vector<std::string> temp;
	print(print::colors::aqua, "Enter username: ");
	std::cin >> usernameTemp;

	for (auto& user : users) temp.push_back(user.first);
	if (std::find(temp.begin(), temp.end(), usernameTemp) != temp.end())
		userLogin(usernameTemp);
	else { 
		print(print::colors::red, "User not found\n");
		userLogic();
	}
}

void userManager::userLogin(std::string username) {
	currentUser = username;
	userIsLogined = true;
}

void userManager::addUserFromData(std::string username, permissionsEC permissions, std::string language, std::string password) {
	users[username] = user(username, permissions, language);
	saveUserData(username);
}

userManager::userManager() { checkOOBE_Passed(); }
bool userManager::userIsYou(std::string username) { return username == currentUser; }
bool userManager::userExist(std::string username) { return users.count(username); }
bool userManager::havePassword(std::string username) { return users[username].havePassword(); }
bool userManager::userHaveAdminPermissions(std::string username) { return users[username].getPermissions() == permissionsEC::admin; }

bool userManager::permissionsCheck(permissionsEC permissions) {
	for (int i = static_cast<int>(permissionsEC::ghost); i <= static_cast<int>(permissionsEC::admin); i++)
		if (permissions == static_cast<permissionsEC>(i)) 
			return true;
	return false;
}

// Права текущего пользователя ниже другого
bool userManager::permissionsHighCurrentUser(std::string username) const { return users[currentUser].getPermissions() < users[username].getPermissions(); }
bool userManager::getUserIsLogined() { return userIsLogined; }
bool userManager::getOOBE_Passed() { return OOBE_Passed; }
std::string userManager::yourUsername() { return currentUser; }

std::map<std::string, std::string> userManager::getUserMap() {
	std::map<std::string, std::string> temp;

	for (auto& element : users)
		temp[element.first] = element.second.getDisplayName();
	return temp;
}

std::map<std::string, std::string> userManager::getLocalVarsMap(std::string username) {
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username))
		return users[username].getAllVars();
	else if (userExist(username) && userIsYou(username)) 
		return users[username].getAllVars();
	return {{"NULL", "NULL"}};
}

std::map<std::string, std::string> userManager::getLanguageMap() {
	std::map<std::string, std::string> temp;

	for (auto& element : users)
		temp[element.first] = element.second.getLanguage();
	return temp;
}

std::map<std::string, permissionsEC> userManager::getPermissionsMap() {
	std::map<std::string, permissionsEC> temp;

	for (auto& element : users)
		temp[element.first] = element.second.getPermissions();
	return temp;
}

void userManager::checkOOBE_Passed() {
	dataManager DM;
	fileManager FM;

	if (FM.fileExist(mainDataFilePath))
		OOBE_Passed = DM.readAllData(mainDataFilePath)["OOBE_Passed"] == "1";
}

void userManager::system_addUser(std::string username) {
	dataManager DM;
	users[username] = user(username, permissionsEC::admin);
	currentUser = username;
	userIsLogined = true;
	DM.createVectorData(usersListFilePath, {username});
	saveUserData(username);
	DM.changeData(mainDataFilePath, "OOBE_Passed", "1");
	checkOOBE_Passed();
}

void userManager::addUser(std::string username, permissionsEC permissions) {
	dataManager DM;
	if (!userExist(username) && permissionsCheck(permissions) && 
	userHaveAdminPermissions(currentUser) && (users.size() < maxUsers)) 
	{
		users[username] = user(username, permissions);
		if (!DM.valueExist(usersListFilePath, username)) { DM.addVectorData(usersListFilePath, username); }
		saveUserData(username);
	}
	else print(print::colors::red, "Failed to create a user\n");
}

void userManager::deleteUser(std::string username) {
	fileManager FM;
	dataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username)) 
	{
		users.erase(username);
		FM.deleteFile(usersFilesPath + username + ".json");
		DM.deleteVectorData(usersListFilePath, username);
	}
	else print(print::colors::red, "This user could not be deleted\n");
}

void userManager::renameUser(std::string username, std::string newUsername) {
	fileManager FM; dataManager DM;
	if (userExist(username) && userHaveAdminPermissions(currentUser) && 
	!permissionsHighCurrentUser(username) && !userIsYou(username)) 
	{
		
		user userTemp = users[username];
		users.erase(username);
		users[newUsername] = userTemp;
		FM.renameFile(usersFilesPath + username + ".json", usersFilesPath + newUsername + ".json");
		DM.changeVectorData(usersListFilePath, username, newUsername);
		DM.changeData(usersFilesPath + newUsername + ".json", "Username", newUsername);
	}
	else print(print::colors::red, "This user could not be renamed\n");
}

void userManager::changePermissionsUser(std::string username, permissionsEC newPermissions) {
	dataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser)) {
		users[username].editPermissions(newPermissions);
		DM.changeData(usersFilesPath + username + ".json", "Permissions", std::to_string(static_cast<int>(newPermissions)));
	}
	else print(print::colors::red, "This user failed to change permissions\n");
}

void userManager::userLists() {
	readAllUsersData();
	std::cout << " - [ Users ] -" << '\n';

	for (auto& user : users)
		std::cout << " - " << user.first << '\n';
	userLogic();
}

void userManager::userLogout() {
	currentUser = ""; userIsLogined = false;
	userLists();
}

void userManager::saveUserData(std::string username) {
	dataManager DM; fileManager FM;
	std::vector<std::string> values = { username, users[username].getDisplayName(), std::to_string(static_cast<int>(users[username].getPermissions())), users[username].getLanguage(), "" };
	FM.createFile(usersFilesPath + username + ".json");
	DM.createData(usersFilesPath + username + ".json", keys, values);
}

void userManager::readUserData(std::string username) {
	dataManager DM;

	if (users[username].getLanguage() != 
		DM.getValue("Data/Users/" + username + ".json","Language"))
			users[username].editLanguage(DM.getValue("Data/Users/" + username + ".json","Language"));
	
	if (users[username].getDisplayName() !=
		DM.getValue("Data/Users/" + username + ".json","Diplay Name"))
			users[username].editDisplayName(DM.getValue("Data/Users/" + username + ".json", "Display Name"));
	
	if (users[username].getPermissions() !=
		static_cast<permissionsEC>(stoi(
			DM.getValue("Data/Users/" + username + ".json","Permissions")
			))) 
				users[username].editPermissions((static_cast<permissionsEC>(stoi(
					DM.getValue("Data/Users/" + username + ".json", "Permissions")
				))));
	
	if (users[username].havePassword() != 
		DM.getValue("Data/Users/" + username + ".json", "Password").empty()) 
			users[username].editPassword(DM.getValue(
				"Data/Users/" + username + ".json", "Password"
			));
}

void userManager::readAllUsersData() {
	fileManager FM;
	dataManager DM;
	for (auto& user : DM.readAllVectorData(usersListFilePath)) {
		if (!userExist(user) && FM.fileExist("Data/Users/" + user + ".json")) 
			addUserFromData(user, static_cast<permissionsEC>(stoi(DM.readAllData(usersFilesPath + user + ".json")["Permissions"])), DM.readAllData(usersFilesPath + user + ".json")["Language"]);
		else if (userExist(user) && FM.fileExist("Data/Users/" + user + ".json")) 
			readUserData(user);
		else 
			print(print::colors::red, "An error occurred while trying to create user '" + user + "' | readAllUsersData()\n");
	}
}