#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include "../extra/VectorTools.cpp"

std::string Current_User;
std::vector<std::string> Users;
std::map<std::string, std::string> Users_Permissions, Users_Language, Users_Passwords;

class UsersManager {
    public:
        bool userExists(std::string UserName) {
            int i = 0;
            bool Temp = false;
            while (i <= (Users.size() - 1)) {
                if (Users[i] == UserName) {
                    Temp = true;
                    break;
                }
                else {
                    Temp = false;
                }
                i++;
            }
            std::cout << Temp << std::endl;
            return Temp;
        };
        bool passwordUserExists(std::string UserName) {
            int i = 0;
            bool Temp = false;
            std::vector<std::string> TempUsersPasswords;
            for (auto &p : Users_Passwords) {
                TempUsersPasswords.push_back(p.first);
            }
            while (i <= (TempUsersPasswords.size() - 1)) {
                if (UserName == TempUsersPasswords[i]) {
                    Temp = true;
                    break;
                }
                else {
                    Temp = false;
                }
                i++;
            }
            return Temp;
        };
        std::string userHavePermissions(std::string UserName) {
            if (userExists(UserName)) {
                return Users_Permissions[UserName];
            }
            else {
                std::cout << "User not found" << std::endl;
                return NULL;
            }
        };
        int UserNumber(std::string UserName) {
            int Temp = -1;
            if (userExists(UserName)) {
                int i = 0;
                while (i <= (Users.size() - 1)) {
                    if (UserName == Users[i]) {
                        Temp = i;
                        break;
                    }
                    i++;
                }
            }
            else {
                Temp = -1;
            }
            return Temp;
        };

        void system_addUser(std::string UserName) {
            Users.push_back(UserName);
            Users_Permissions[UserName] = "Root";
            Users_Language[UserName] = "English";
            Current_User = UserName;
        }
        void addUser(std::string UserName, std::string Language, std::string Permissions) {
            if (userHavePermissions(Current_User) == "Admin" || userHavePermissions(Current_User) == "Root") {
                Users.push_back(UserName);
                Users_Permissions[UserName] = Permissions;
                Users_Language[UserName] = Language;
            }
            else {
                std::cout << "No" << std::endl;
            }
        }
        /*
        void removeUser(std::string UserName) {
            if (userExists(UserName)) {
                if (userHavePermissions(Current_User) == "Admin" || userHavePermissions(Current_User) == "Root") {
                    if (userHavePermissions(Current_User) == "Admin" && userHavePermissions(UserName) == "Root") {

                    }
                    else if (Current_User == UserName) {
                        std::cout << "No" << std::endl;
                    }
                    else {
                        //removeVectorVar(Users, UserNumber(UserName));
                        Users_Language.erase(UserName);
                        Users_Permissions.erase(UserName);
                        if (passwordUserExists(UserName)) {
                            Users_Passwords.erase(UserName);
                        }
                    }
                }
            }
            else {
                
            }
        }
        */
        void userSetPermissions(std::string UserName, std::string Permissions) {
            if (userExists(UserName)) {
                if (userHavePermissions(Current_User) == "Admin" || userHavePermissions(Current_User) == "Root") {
                    if (Current_User == UserName) {
                        
                    }
                    else if (userHavePermissions(Current_User) == "Admin" && userHavePermissions(UserName) == "Root") {

                    }
                    else {
                        Users_Permissions[UserName] = Permissions;
                    }
                }
            }
        }
};

void OOBE() {
    UsersManager UM;
    std::string firstUsername;
    std::cout << "Enter new username: ";
    std::cin >> firstUsername;
    UM.system_addUser(firstUsername);
}