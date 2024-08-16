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
#ifndef NRC_BASE_USERS_USER_H_
#define NRC_BASE_USERS_USER_H_

#include <string>
#include "Core/extra/variables.h"
#include "permissions_enum_class.h"

class user {
    private:
        std::map<std::string, variableData> localVariables;
        std::string displayName, language, password;
        permissionsEC permissions;
        bool userCreated = false;
        bool varExist(std::string name);
    public:
        user();
        user(std::string displayName, permissionsEC permissions, std::string language = "English", std::string password = "");
        
        void addVar(std::string name, std::string description, std::function<void()> varFunction);

        void editDisplayName(std::string newDisplayName);
        void editLanguage(std::string newLanguage);
        void editPassword(std::string password);
        void editPermissions(permissionsEC newPermissions);
        void editVarFunction(std::string name, std::function<void()> function);
        void editVarDescription(std::string name, std::string description);
        void renameVar(std::string oldName, std::string newName);

        bool havePassword();
        bool truePassword(std::string password);
        //bool trueKey(std::string longUserKey);

        std::map<std::string, std::string> getAllVars();
        std::string getVarName();
        std::string getVarDescription();
        std::string getDisplayName();
        std::string getLanguage();
        permissionsEC getPermissions();

        std::function<void()> varFuncStart(std::string name);
};

#endif