#include <iostream>
#include "Core/base/print.h"
#include "Core/base/data/data_manager.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/filesystem/pseudo_fs.h"
#include "Core/experimental/event_manager.h"

void core::UserManager::userLogic() {
	std::string usernameTemp; std::vector<std::string> temp;
	print(colors::aqua, "Enter username: ");
    while (!(std::cin >> std::ws)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
	std::getline(std::cin, usernameTemp);

	for (auto& user : users) temp.push_back(user.getUsername());
	if (std::find(temp.begin(), temp.end(), usernameTemp) != temp.end())
		userLogin(usernameTemp);
	else { 
		print(colors::red, "User not found\n");
		userLogic();
	}
}

void core::UserManager::userLogin(const std::string& username) {
	if (havePassword(username)) {
		bool passwordNotPassed = true;
		while (passwordNotPassed) {
			std::string password;
			print(colors::aqua, "Enter password: ");
			while (!(std::cin >> std::ws)) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			std::getline(std::cin, password);

			if (!users[userVectorPos(username)].truePassword(password))
				print(colors::red, "Wrong password!\n");
			else
				passwordNotPassed = false;
		}
	}
	currentUser = username;
	userIsLogined = true;
}

void core::UserManager::addUserFromData(const std::string& username, const std::string& displayName, const permissionsEC& permissions, const std::string& language, const std::string& password) {
	users.push_back(User(username, permissions, language, password));
	users[userVectorPos(username)].editDisplayName(displayName);
	core::EventManager EM;

	core::structDataEvents::UserAddEvent eventData = {
		username,
		users[userVectorPos(username)].getDisplayName(),
		users[userVectorPos(username)].getPermissions(),
		static_cast<size_t>(userVectorPos(username))
	};

	EM.eventsStart(USER_ADD_EVENT, eventData);
	core::PseudoFS FS;
	if (FS.getFolderData("./home/" + username, true).name.empty()) FS.createFolder("./home/" + username);
	FS.savePFS();
	saveUserData(username);
}

core::UserManager::UserManager() { checkOOBE_Passed(); }
bool core::UserManager::userIsYou(const std::string& username) { return username == currentUser; }
bool core::UserManager::userExist(const std::string& username) {
	for (User& user : users) {
		if (user.getUsername() == username) return true;
	}
	return false;
}
bool core::UserManager::havePassword(const std::string& username) { return users[userVectorPos(username)].havePassword(); }
bool core::UserManager::userHaveAdminPermissions(const std::string& username) { return users[userVectorPos(username)].getPermissions() == permissionsEC::admin; }

bool core::UserManager::permissionsCheck(const permissionsEC& permissions) {
	for (int i = static_cast<int>(permissionsEC::ghost); i <= static_cast<int>(permissionsEC::admin); i++)
		if (permissions == static_cast<permissionsEC>(i)) 
			return true;
	return false;
}

bool core::UserManager::permissionsHighCurrentUser(const std::string& username) { 
	return users[userVectorPos(currentUser)].getPermissions() < users[userVectorPos(username)].getPermissions();
}

bool core::UserManager::getUserIsLogined() { return userIsLogined; }
bool core::UserManager::getOOBE_Passed() { return OOBE_Passed; }

int core::UserManager::userVectorPos(const std::string& username) {
	int realUsersSize = users.size() - (users.size() == 1 ? 0 : 1);
	for (int i = 0; i <= realUsersSize; i++) {
		if (users[i].getUsername() == username) return i;
	}
	return -1;
}

core::User &core::UserManager::currentUserData() {
	return users[userVectorPos(currentUser)];
}
const std::string core::UserManager::yourUsername() { return currentUser; }

std::map<std::string, std::string> core::UserManager::getUserMap() {
	std::map<std::string, std::string> temp;

	for (User& user : users)
		temp[user.getUsername()] = user.getDisplayName();
	return temp;
}

std::map<std::string, std::string> core::UserManager::getLocalVarsMap(const std::string& username) {
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username))
		return users[userVectorPos(username)].getAllVars();
	else if (userExist(username) && userIsYou(username)) 
		return users[userVectorPos(username)].getAllVars();
	return {};
}

std::map<std::string, std::string> core::UserManager::getLanguageMap() {
	std::map<std::string, std::string> temp;

	for (User& user : users)
		temp[user.getUsername()] = user.getLanguage();
	return temp;
}

std::map<std::string, core::permissionsEC> core::UserManager::getPermissionsMap() {
	std::map<std::string, permissionsEC> temp;

	for (User& user : users)
		temp[user.getUsername()] = user.getPermissions();
	return temp;
}

void core::UserManager::checkOOBE_Passed() {
	DataManager DM;
	FileManager FM;

	if (FM.fileExist(mainDataFilePath))
		OOBE_Passed = DM.readAllData(mainDataFilePath)["OOBE_Passed"] == "1";
}

void core::UserManager::system_addUser(const std::string& username) {
	DataManager DM;
	users.push_back(User(username, permissionsEC::admin));
	currentUser = username;
	userIsLogined = true;
	DM.createVectorData(usersListFilePath, {username});
	core::EventManager EM;

	core::structDataEvents::UserAddEvent eventData = {
		username,
		users[userVectorPos(username)].getDisplayName(),
		users[userVectorPos(username)].getPermissions(),
		static_cast<size_t>(userVectorPos(username))
	};

	EM.eventsStart(USER_ADD_EVENT, eventData);

	core::PseudoFS FS;
	if (FS.getFolderData("./home/" + username, true).name.empty()) FS.createFolder("./home/" + username);
	FS.savePFS();
	saveUserData(username);
	DM.changeData(mainDataFilePath, "OOBE_Passed", "1");
	checkOOBE_Passed();
}

void core::UserManager::addUser(const std::string& username, const permissionsEC& permissions) {
	DataManager DM;
	if (!userExist(username) && permissionsCheck(permissions) && 
	userHaveAdminPermissions(currentUser) && (users.size() < static_cast<size_t>(maxUsers))) 
	{
		users.push_back(User(username, permissions));
		if (!DM.valueExist(usersListFilePath, username)) { DM.addVectorData(usersListFilePath, username); }
		core::EventManager EM;

		core::structDataEvents::UserAddEvent eventData = {
			username,
			users[userVectorPos(username)].getDisplayName(),
			users[userVectorPos(username)].getPermissions(),
			static_cast<size_t>(userVectorPos(username))
		};

		EM.eventsStart(USER_ADD_EVENT, eventData);
		core::PseudoFS FS;
		if (FS.getFolderData("./home/" + username, true).name.empty()) FS.createFolder("./home/" + username);
		FS.savePFS();
		saveUserData(username);
	}
	else print(colors::red, "Failed to create a user\n");
}

void core::UserManager::deleteUser(const std::string& username) {
	FileManager FM;
	DataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && 
	userHaveAdminPermissions(currentUser) && !userIsYou(username)) 
	{
		auto iter = users.begin();
		for (User& user : users) {
			if (user.getUsername() == username) {
				core::EventManager EM;

				core::structDataEvents::UserDeleteEvent eventData = {
					username,
					users[userVectorPos(username)].getDisplayName(),
					users[userVectorPos(username)].getPermissions()
				};

				EM.eventsStart(USER_DELETE_EVENT, eventData);

				users.erase(iter);
				core::PseudoFS FS;
				if (!FS.getFolderData("./home/" + username, true).name.empty()) FS.deleteFolder("./home/" + username);
				FS.savePFS();
				break;
			}
			iter++;
		}
		FM.deleteFile(usersPath + username + ".json");
		DM.deleteVectorData(usersListFilePath, username);
	}
	else print(colors::red, "This user could not be deleted\n");
}

void core::UserManager::renameUser(const std::string& username, const std::string& newUsername) {
	FileManager FM; DataManager DM;
	if (userExist(username) && userHaveAdminPermissions(currentUser) && 
	!permissionsHighCurrentUser(username) && !userIsYou(username)) 
	{
		
		User userTemp = users[userVectorPos(username)];
		auto iter = users.begin();
		for (User& user : users) {
			if (user.getUsername() == username) {
				users.erase(iter);
				break;
			}
			iter++;
		}
		userTemp.editUsername(newUsername);
		users.push_back(userTemp);
		FM.renameFile(usersPath + username + ".json", usersPath + newUsername + ".json");
		DM.changeVectorData(usersListFilePath, username, newUsername);
		DM.changeData(usersPath + newUsername + ".json", "Username", newUsername);
		core::EventManager EM;

		core::structDataEvents::UserChangeEvent eventData = {
			username, newUsername,
			userTemp.getDisplayName(), userTemp.getDisplayName(),
			userTemp.getPermissions(), userTemp.getPermissions(),
			static_cast<size_t>(userVectorPos(username))
		};

		EM.eventsStart(USER_CHANGE_EVENT, eventData);
	}
	else print(colors::red, "This user could not be renamed\n");
}

void core::UserManager::changePermissionsUser(const std::string& username, const permissionsEC& newPermissions) {
	DataManager DM;
	if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser)) {
		core::permissionsEC past = users[userVectorPos(username)].getPermissions();
		users[userVectorPos(username)].editPermissions(newPermissions);
		DM.changeData(usersPath + username + ".json", "Permissions", std::to_string(static_cast<int>(newPermissions)));
		core::EventManager EM;

		core::structDataEvents::UserChangeEvent eventData = {
			username, username,
			users[userVectorPos(username)].getDisplayName(), users[userVectorPos(username)].getDisplayName(),
			past, users[userVectorPos(username)].getPermissions(),
			static_cast<size_t>(userVectorPos(username))
		};

		EM.eventsStart(USER_CHANGE_EVENT, eventData);	
	}
	else print(colors::red, "This user failed to change permissions\n");
}

void core::UserManager::userLists() {
	readAllUsersData();
	std::cout << " - [ Users ] -" << '\n';

	for (auto& user : users)
		std::cout << " - " << user.getUsername() << '\n';
	userLogic();
}

void core::UserManager::userLogout() {
	currentUser = ""; userIsLogined = false;
	userLists();
}

void core::UserManager::saveUserData(const std::string& username) {
	DataManager DM; FileManager FM;
	int userPos = userVectorPos(username);
	std::vector<std::string> values = { username, users[userPos].getDisplayName(), std::to_string(static_cast<int>(users[userPos].getPermissions())), users[userPos].getLanguage(), users[userPos].getPassword() };
	FM.createFile(usersPath + username + ".json");
	DM.createData(usersPath + username + ".json", keys, values);
}

void core::UserManager::readUserData(const std::string& username) {
	DataManager DM;
	int userPos = userVectorPos(username);
	if (userPos == -1) return;

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

void core::UserManager::readAllUsersData() {
	FileManager FM;
	DataManager DM;
	for (const std::string& user : DM.readAllVectorData(usersListFilePath)) {
		if (!userExist(user) && FM.fileExist("Data/Users/" + user + ".json")) 
			addUserFromData(user, DM.readAllData(usersPath + user + ".json")["Display Name"], static_cast<permissionsEC>(stoi(DM.readAllData(usersPath + user + ".json")["Permissions"])), DM.readAllData(usersPath + user + ".json")["Language"], DM.readAllData(usersPath + user + ".json")["Password"]);
		else if (userExist(user) && FM.fileExist("Data/Users/" + user + ".json")) 
			readUserData(user);
		else 
			print(colors::red, "An error occurred while trying to create user '" + user + "'\n");
	}
}