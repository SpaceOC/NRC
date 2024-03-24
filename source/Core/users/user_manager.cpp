#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

class userManager {
    private:
        static inline bool userNotLogout = false;
        static inline std::string currentUser;
        static inline std::map<std::string, std::string> users;
        static inline std::map<std::string, std::string> usersPermissions;
        static inline std::map<std::string, std::string> usersPasswords;
    public:
        bool userIsYou(std::string username) const {
            if (username == currentUser && userNotLogout) {
                return true;
            }
            else {
                return false;
            }
        }

        bool userExist(std::string username) const {
            if (users.count(username)) {
                return true;
            }
            else {
                std::cout << "This user doesn't exist" << std::endl;
                return false;
            }
        }

        bool havePassword(std::string username) const {
            if (usersPasswords.count(username)) {
                return true;
            }
            else {
                return false;
            }
        }

        bool permissionsCheck(std::string permissions) const {
            if (permissions == "Ghost" || permissions == "User" || permissions == "Admin" || permissions == "Root") {
                return true;
            }
            else {
                std::cout << "This type of authorization does not exist" << std::endl;
                return false;
            }
        }

        // Права текущего пользователя ниже другого
        bool permissionsHighCurrentUser(std::string username) const {
            if (((usersPermissions[currentUser] == "Ghost" || usersPermissions[currentUser] == "User") && (username == "Root" || username == "Admin")) || (usersPermissions[currentUser] == "Admin" && username == "Root")) {
                std::cout << "The current user has less rights than the other user" << std::endl;
                return true;
            }
            else {
                return false;
            }
        }

        std::string YourUsername() const {
            return currentUser;
        }

        void getAllInfoUsers() {
            std::cout << "--- [ User - Permissions --- ] " << std::endl;
            for (auto& element : usersPermissions) {
                std::cout << element.first + " - " + element.second << '\n';
            }
        }

        std::vector<std::string> getInfoUser(std::string username) {
            if (userExist(username)) {
                std::vector<std::string> Temp;
                Temp.push_back("User: "+ username);
                Temp.push_back("Permissions: " + usersPermissions[username]);
                return Temp;
            }
        }

        void system_addUser(std::string username) const {
            users[username] = username;
            usersPermissions[username] = "Root";
            currentUser = users[username];
            userNotLogout = true;
        }

        void addUser(std::string username, std::string permissions) const {
            if (!userExist(username) && permissionsCheck(permissions)) {
                users[username] = username;
                usersPermissions[username] = permissions;
            }
            else {
                std::cout << "Failed to create a user" << std::endl;
            }
        }

        void deleteUser(std::string username) const {
            try {
                if (userExist(username) && !permissionsHighCurrentUser(username) && !userIsYou(username)) {
                    users.erase(username);
                    usersPermissions.erase(username);
                    if (havePassword(username)) { usersPasswords.erase(username); }
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
                if (userExist(username) && !permissionsHighCurrentUser(username) && !userIsYou(username)) {
                    users.erase(username);
                    users[new_username] = new_username;
                    usersPermissions[new_username] = usersPermissions[username];
                    usersPermissions.erase(username);
                    if (havePassword(username)) { usersPasswords[new_username] = usersPasswords[username]; usersPasswords.erase(username); }
                }
            else {
                std::cout << "This user could not be renamed" << std::endl;
            }
        }

        void changePermissionsUser(std::string username, std::string newPermissions) {
            if (userExist(username) && !permissionsHighCurrentUser(username) && permissionsCheck(newPermissions)) {
                usersPermissions[username] = newPermissions;
            }
            else {
                std::cout << "This user failed to change permissions" << std::endl;
            }
        }
};