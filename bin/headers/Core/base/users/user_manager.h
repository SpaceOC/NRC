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
#ifndef NRC_BASE_USERS_USER_MANAGER_H_
#define NRC_BASE_USERS_USER_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include "user.h"

namespace core {
	// User Management in the NRC.
	class UserManager {
		private:
			static inline bool OOBEPassed;
			static inline bool userIsLogined; // Whether the user is logged in to their account
			static inline int maxUsers = 10; // Maximum number of users.
			static inline std::string currentUser; // Current username
			static inline std::vector<User> users;
			static inline const std::string mainDataFilePath = "Data/MainData.json";
			static inline const std::string usersListFilePath = "Data/Users.json";
			static inline const std::string usersPath = "Data/Users/";

			// Json keys for user data files. Used when a new user data file is created.
			static inline const std::vector<std::string> keys = { "Username", "Display Name", "Permissions", "Language", "Password" };
			
			// Selecting an account and logging into it (logging into the account is done in the userLogin function).
			static void userLogic();

			// Log in to the selected user's account.
			static void userLogin(const std::string& username);

			// Creates a new user from the user file data.
			static void addUserFromData(const std::string& username, const std::string& displayName, const Permissions& permissions, const std::string& language, const std::string& password = "");
		public:
			UserManager();
			static bool userIsYou(const std::string& username);
			static bool userExist(const std::string& username);
			static bool havePassword(const std::string& username);
			static bool userHaveAdminPermissions(const std::string& username);
			static bool permissionsCheck(const Permissions& permissions);
			// The current user's permissions are lower than the other user's
			static bool permissionsHighCurrentUser(const std::string& username);
			static bool getUserIsLogined();
			static bool getOOBEPassed();

			/*
			* Returns the value of the position (as a number) in the vector if the user exists. Otherwise returns -1.
			* @param username Username
			* @return Position (if user exists) or -1
			*/
			static int userVectorPos(const std::string& username);
			static User &currentUserData();
			static const std::string yourUsername();
			static std::map<std::string, std::string> getUserMap();
			static std::map<std::string, std::string> getLocalVarsMap(const std::string& username);
			static std::map<std::string, std::string> getLanguageMap();
			static std::map<std::string, Permissions> getPermissionsMap();

			// Checks the value of OOBE_Passed in the MainData.json file (which is in the Data folder) and changes the value of OOBE_Passed to true or false depending on the value.
			static void checkOOBE();

			/*
			* Create a new user with administrator rights.
			* @param username Username
			*/
			static void systemAddUser(const std::string& username);

			/*
			* Create a new user with specific permissions.
			* @param username Username
			* @param permissions Permissions
			*/
			static void addUser(const std::string& username, const Permissions& permissions);

			/*
			* User deletion
			* @param username Username
			*/
			static void deleteUser(const std::string& username);

			/*
			* User renaming
			* @param username Username
			* @param newUsername New username
			*/
			static void renameUser(const std::string& username, const std::string& newUsername);

			/*
			* Changing user permissions
			* @param username Username
			* @param newPermissions New permissions
			*/
			static void changePermissionsUser(const std::string& username, const Permissions& newPermissions);
			
			/* 
			* Outputs the entire list of users that are in the NRC.
			* After outputting the list of users, immediately runs the userLogic function.
			*/			
			static void userLists();

			/* 
			* Logout from the current user after which the userLists function is started.
			*/
			static void userLogout();

			/* 
			* Saves user data (username, display name, permissions, password, language) to a separate file.
			*/
			static void saveUserData(const std::string& username);

			/* 
			* Reads user data (username, display name, permissions, password, language) and updates the user if their data does not match the data in the file.
			*/
			static void readUserData(const std::string& username);
			
			/* 
			* Reads the Users.json file (which is in the Data folder) then checks the files in the Users folder (which are in the Data folder).
			* If the file with user data exists, but the user is not in the users vector yet, then a user with this data is created (using the addUserFromData function).
			* If the user exists and his file also exists, then the user data is simply updated (using the readUserData function).
			*/
			static void readAllUsersData();
	};
}

#endif