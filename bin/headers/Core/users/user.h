#pragma once
#ifndef USER
#define USER

#include <string>
#include "permissions_enum_class.h"

class user {
    private:
        std::string displayName, language, password;
        permissionsEC permissions;
        bool userCreated = false;
    public:
        user();
        user(std::string displayName, permissionsEC permissions, std::string language = "English", std::string password = "");
        void editDisplayName(std::string newDisplayName);
        void editLanguage(std::string newLanguage);
        void editPassword(std::string password);
        void editPermissions(permissionsEC newPermissions);
        bool havePassword();
        bool truePassword(std::string password);
        //bool trueKey(std::string longUserKey);
        std::string getDisplayName();
        std::string getLanguage();
        permissionsEC getPermissions();
};
#endif