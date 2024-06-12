#pragma once

#include <string>
#include <vector>
#include <map>


class userManager {
private:
	static inline bool OOBE_Passed;
	static inline bool userIsLogined; // Вошёл ли пользователь в свой аккаунт
	static inline bool shutUp;
	static inline int maxUsers;
	static inline std::string currentUser;
	static inline std::map<std::string, std::string> users;
	static inline std::map<std::string, std::string> usersPermissions;
	static inline std::map<std::string, std::string> usersLanguages;
	static inline std::map<std::string, std::string> usersPasswords;

	const std::string mainDataFilePath = "Data/MainData.json";
	const std::string usersListFilePath = "Data/Users.json";
	const std::string usersFilesPath = "Data/Users/";
	const std::vector<std::string> keys = { "Username", "Permissions", "Language", "Password" };

	void userLists();

	void userLogic();

	void userLogin(std::string username);

	void system_addUser(std::string username, std::string permissions, std::string language);

	void system_readUserData(std::string username);
public:
	userManager();

	bool userLangExist(std::string username);

	bool userIsYou(std::string username);

	bool userExist(std::string username);

	bool havePassword(std::string username);

	bool userHaveAdminPermissions(std::string username);

	bool permissionsCheck(std::string permissions);

	// Права текущего пользователя ниже другого
	bool permissionsHighCurrentUser(std::string username) const;

	bool getOOBE_Passed();

	size_t numberofUsers();

	std::string YourUsername();

	std::map<std::string, std::string> getUserMap();

	std::map<std::string, std::string> getLanguageMap();

	std::map<std::string, std::string> getPermissionsMap();

	void checkOOBE_Passed();

	std::vector<std::string> getUserInfo(std::string username);

	void system_addUser(std::string username);

	void addUser(std::string username, std::string permissions);

	void deleteUser(std::string username);

	void renameUser(std::string username, std::string new_username);

	void changePermissionsUser(std::string username, std::string newPermissions);

	void userListManagerStart();
	void userLogout();

	void saveUserData(std::string username) const;

	void readUserData(std::string username) const;

	void readAllUsersData();
};