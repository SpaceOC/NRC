#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "../data/data_manager.cpp"

class userManager {
    private:
        static inline bool OOBE_Passed = false;
        static inline bool userIsLogined = false; // Вошёл ли пользователь в свой аккаунт
        static inline bool shutUp = false;
        static inline int maxUsers = 20;
        static inline std::string currentUser;
        static inline std::map<std::string, std::string> users;
        static inline std::map<std::string, std::string> usersPermissions;
        static inline std::map<std::string, std::string> usersLanguages;
        static inline std::map<std::string, std::string> usersPasswords;

        const std::string mainDataFilePath = "Data/MainData.json";
        const std::string usersListFilePath = "Data/Users.json";
        const std::string usersFilesPath = "Data/Users/";
        const std::vector<std::string> keys = {"Username", "Permissions", "Language", "Password"};

        void userLists() {
            readAllUsersData();
            std::cout << " [ Users ]" << std::endl;
            for (auto& user : users) {
                std::cout << user.first << std::endl;
            }
            userLogic();
        }

        void userLogic() {
            std::string usernameTemp; std::vector<std::string> temp;
            std::cout << "Enter username: ";
            std::cin >> usernameTemp;
            for (auto& user : users) { temp.push_back(user.first); }
            if (std::find(temp.begin(), temp.end(), usernameTemp )!= temp.end()) { userLogin(usernameTemp); } 
            else { std::cout << "User not found" << std::endl; }
        }

        void userLogin(std::string username) {
            currentUser = username; userIsLogined = true;
        }
    public:
        userManager() {
            checkOOBE_Passed();
        }

        bool userIsYou(std::string username) {
            return username == currentUser;
        }

        bool userExist(std::string username) {
            return users.count(username);
        }

        bool havePassword(std::string username) {
            return usersPasswords.count(username);
        }

        bool userHaveAdminPermissions(std::string username) {
            return usersPermissions[username] == "Admin" || usersPermissions[username] == "Root";
        }

        bool permissionsCheck(std::string permissions) {
            return permissions == "Ghost" || permissions == "User" || permissions == "Admin" || permissions == "Root";
        }

        // Права текущего пользователя ниже другого
        bool permissionsHighCurrentUser(std::string username) const {
            return ((usersPermissions[currentUser] == "Ghost" || usersPermissions[currentUser] == "User") && (username == "Root" || username == "Admin")) || (usersPermissions[currentUser] == "Admin" && username == "Root");
        }
        
        bool getOOBE_Passed() {
            return OOBE_Passed;
        }

        size_t numberofUsers() {
            return users.size();
        }

        std::string YourUsername() {
            return currentUser;
        }

        void checkOOBE_Passed() {
            dataManager DM;
            fileManager FM;
            if (FM.fileExist(mainDataFilePath)) {
                if (DM.readAllData(mainDataFilePath)["OOBE_Passed"] == "1") { OOBE_Passed = true; }
                else { OOBE_Passed = false; }
            }
        }

        void getAllInfoUsers() {
            for (auto& element : usersPermissions) {
                std::cout << element.first + " - " + element.second << '\n';
            }
        }

        std::vector<std::string> getUserInfo(std::string username) {
            std::vector<std::string> temp;
            temp.push_back(username);
            temp.push_back(usersPermissions[username]);
            return temp;
        }

        void system_addUser(std::string username, std::string permissions) {
            dataManager DM;
            users[username] = username;
            usersPermissions[username] = permissions;
            if (!OOBE_Passed) { currentUser = username; }
            userIsLogined = true;
            if (!OOBE_Passed) { DM.createData(usersListFilePath, username, ""); }
            saveUserData(username);
            if (!OOBE_Passed) { DM.changeData(mainDataFilePath, "OOBE_Passed", "1", false); checkOOBE_Passed(); }
        }

        void addUser(std::string username, std::string permissions) {
            dataManager DM;
            if (!userExist(username) && permissionsCheck(permissions) && userHaveAdminPermissions(currentUser) && (numberofUsers() < maxUsers)) {
                users[username] = username;
                usersPermissions[username] = permissions;
                if (!DM.keyExist(usersListFilePath, username)) { DM.addData(usersListFilePath, username, ""); }
                saveUserData(username);
            }
            else {
                std::cout << "Failed to create a user" << std::endl;
            }
        }

        void deleteUser(std::string username) {
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
            catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
            
        }

        void renameUser(std::string username, std::string new_username) {
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

        void changePermissionsUser(std::string username, std::string newPermissions) {
            dataManager DM;
            if (userExist(username) && !permissionsHighCurrentUser(username) && userHaveAdminPermissions(currentUser) && permissionsCheck(newPermissions)) {
                usersPermissions[username] = newPermissions;
                DM.changeData(usersFilesPath + username + ".json", "Permissions", newPermissions, false);
            }
            else {
                std::cout << "This user failed to change permissions" << std::endl;
            }
        }

        void userListManagerStart() {
            userLists();
        }

        void userLogout() {
            currentUser = ""; userIsLogined = false;
            userLists();
        }

        void saveUserData(std::string username) {
            dataManager DM; fileManager FM;
            std::vector<std::string> values = {username, usersPermissions[username], "English", ""};
            FM.createFile(usersFilesPath + username + ".json");
            DM.createData(usersFilesPath + username + ".json", keys, values);
        }

        void readUserData(std::string username) {
            dataManager DM;
            system_addUser(username, DM.readAllData(usersFilesPath + username + ".json")["Permissions"]);
        }

        void readAllUsersData() {
            dataManager DM;
            for (auto& user : DM.readAllData(usersListFilePath)) {
                readUserData(user.first);
            }
        }
};