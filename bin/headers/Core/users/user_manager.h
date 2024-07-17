#pragma once
#ifndef USER_MANAGER
#define USER_MANAGER

#include <string>
#include <vector>
#include <map>
#include "Core/users/user.h"

class userManager {
	private:
		static inline bool OOBE_Passed;
		static inline bool userIsLogined; // Вошёл ли пользователь в свой аккаунт
		static inline bool shutUp;
		static inline int maxUsers = 10;
		static inline std::string currentUser;
		static inline std::map<std::string, user> users;
		const std::string mainDataFilePath = "Data/MainData.json";
		const std::string usersListFilePath = "Data/Users.json";
		const std::string usersFilesPath = "Data/Users/";
		const std::vector<std::string> keys = { "Username", "Display Name", "Permissions", "Language", "Password" };
		void userLists();
		void userLogic();
		void userLogin(std::string username);
		void addUserFromData(std::string username, permissionsEC permissions, std::string language, std::string password = "");
	public:
		userManager();
		bool userIsYou(std::string username);
		bool userExist(std::string username);
		bool havePassword(std::string username);
		bool userHaveAdminPermissions(std::string username);
		bool permissionsCheck(permissionsEC permissions);
		// Права текущего пользователя ниже другого
		bool permissionsHighCurrentUser(std::string username) const;
		bool getOOBE_Passed();
		size_t numberofUsers();
		std::string yourUsername();
		std::map<std::string, std::string> getUserMap();
		std::map<std::string, std::string> getLanguageMap();
		std::map<std::string, permissionsEC> getPermissionsMap();
		void checkOOBE_Passed();
		void system_addUser(std::string username);
		void addUser(std::string username, permissionsEC permissions);
		void deleteUser(std::string username);
		void renameUser(std::string username, std::string newUsername);
		void changePermissionsUser(std::string username, permissionsEC newPermissions, std::string password = "");
		void userListManagerStart();
		void userLogout();
		void saveUserData(std::string username) const;
		void readUserData(std::string username) const;
		void readAllUsersData();
};
#endif