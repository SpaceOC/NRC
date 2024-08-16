/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#ifndef NRC_BASE_USERS_USER_MANAGER_H_
#define NRC_BASE_USERS_USER_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include "user.h"

class userManager {
	private:
		static inline bool OOBE_Passed;
		static inline bool userIsLogined; // Whether the user is logged in to their account
		static inline int maxUsers = 10;
		static inline std::string currentUser;
		static inline std::map<std::string, user> users;
		const std::string mainDataFilePath = "Data/MainData.json";
		const std::string usersListFilePath = "Data/Users.json";
		const std::string usersFilesPath = "Data/Users/";
		const std::vector<std::string> keys = { "Username", "Display Name", "Permissions", "Language", "Password" };
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
		// The current user's permissions are lower than the other user's
		bool permissionsHighCurrentUser(std::string username) const;
		bool getUserIsLogined();
		bool getOOBE_Passed();
		std::string yourUsername();
		std::map<std::string, std::string> getUserMap();
		std::map<std::string, std::string> getLocalVarsMap(std::string username);
		std::map<std::string, std::string> getLanguageMap();
		std::map<std::string, permissionsEC> getPermissionsMap();
		void checkOOBE_Passed();
		void system_addUser(std::string username);
		void addUser(std::string username, permissionsEC permissions);
		void deleteUser(std::string username);
		void renameUser(std::string username, std::string newUsername);
		void changePermissionsUser(std::string username, permissionsEC newPermissions);
		void userLists();
		void userLogout();
		void saveUserData(std::string username);
		void readUserData(std::string username);
		void readAllUsersData();
};
#endif