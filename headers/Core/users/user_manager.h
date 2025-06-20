#ifndef NRC_BASE_USERS_USER_MANAGER_H_
#define NRC_BASE_USERS_USER_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include "Core/settings.h"
#include "Core/users/user_permissions_enum.h"

namespace core {
	struct VariableData;
	class User;
	class VariablesManager;

	// User Management in the NRC.
	class UserManager {
		private:
			bool OOBEPassed;
			bool userIsLogined; // Whether the user is logged in to their account
			const int maxUsers = 10; // Maximum number of users.
			std::string currentUser; // Current username
			std::vector<User*> users;
			const std::string mainDataFilePath = "Data/main.json";
			const std::string usersPath = "Data/Users/";
			
			// Selecting an account and logging into it (logging into the account is done in the userLogin function).
			void userLogic();

			#ifndef NRC_WEB
			// Log in to the selected user's account.
			void userLogin(const std::string& username);
			#endif

			// Creates a new user from the user file data.
			void addUserFromData(const std::string& username, const std::string& displayName, const UserPermissions& permissions, const std::string& language, const std::string& password = "");
		public:
			UserManager();
			bool userIsYou(const std::string& username);
			bool userExist(const std::string& username);
			bool havePassword(const std::string& username);
			bool userHaveAdminPermissions(const std::string& username);
			// The current user's permissions are lower than the other user's
			bool permissionsHighCurrentUser(const std::string& username);
			bool getUserIsLogined();
			bool getOOBEPassed();

			#ifdef NRC_WEB
			// Log in to the selected user's account.
			void userLogin(const std::string& username, const std::string& password);
			#endif

			/*
			* Returns the value of the position (as a number) in the vector if the user exists. Otherwise returns -1.
			* @param username Username
			* @return Position (if user exists) or -1
			*/
			int userVectorPos(const std::string& username);
			User &currentUserData();
			User &getUser(const std::string& username);
			const std::string yourUsername();
			std::map<std::string, std::string> getUserMap();
			std::vector<core::VariableData> getLocalVarsMap(const std::string& username);
			std::map<std::string, std::string> getLanguageMap();
			std::map<std::string, UserPermissions> getPermissionsMap();

			// Checks the value of OOBE_Passed in the MainData.json file (which is in the Data folder) and changes the value of OOBE_Passed to true or false depending on the value.
			void checkOOBE();

			/*
			* Create a new user with administrator rights.
			* @param username Username
			*/
			void systemAddUser(const std::string& username);

			/*
			* Create a new user with specific permissions.
			* @param username Username
			* @param permissions Permissions
			*/
			void addUser(const std::string& username, const UserPermissions& permissions);

			/*
			* User deletion
			* @param username Username
			*/
			void deleteUser(const std::string& username);

			/*
			* User renaming
			* @param username Username
			* @param newUsername New username
			*/
			void renameUser(const std::string& username, const std::string& newUsername);

			/*
			* Changing user permissions
			* @param username Username
			* @param newPermissions New permissions
			*/
			void changePermissionsUser(const std::string& username, const UserPermissions& newPermissions);

			/* 
			* Outputs the entire list of users that are in the NRC.
			* After outputting the list of users, immediately runs the userLogic function.
			*/			
			void userLists();

			/* 
			* Logout from the current user after which the userLists function is started.
			*/
			void userLogout();

			/* 
			* Saves user data (username, display name, permissions, password, language) to a separate file.
			*/
			void saveUserData(const std::string& username);

			/* 
			* Reads user data (username, display name, permissions, password, language) and updates the user if their data does not match the data in the file.
			*/
			void readUserData(const std::string& username);
			
			/* 
			* Reads the Users.json file (which is in the Data folder) then checks the files in the Users folder (which are in the Data folder).
			* If the file with user data exists, but the user is not in the users vector yet, then a user with this data is created (using the addUserFromData function).
			* If the user exists and his file also exists, then the user data is simply updated (using the readUserData function).
			*/
			void readAllUsersData();
	};

	extern UserManager* userManager();
}

#endif