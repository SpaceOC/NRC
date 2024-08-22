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

namespace core {
    class user {
        private:
            std::map<std::string, variableData> localVariables;
            std::string username, displayName, language, password;
            permissionsEC permissions;
            bool userCreated = false;
            bool varExist(const std::string& name);
        public:
            user();
            user(const std::string& username, const permissionsEC& permissions, const std::string& language = "English", const std::string& password = "");
            
            void addVar(const std::string& name, const std::string& description, const std::function<void()>& varFunction);

            void editUsername(const std::string& newUsername);
            void editDisplayName(const std::string& newDisplayName);
            void editLanguage(const std::string& newLanguage);
            void editPassword(const std::string& password);
            void editPermissions(const permissionsEC& newPermissions);
            void editVarFunction(const std::string& name, const std::function<void()>& function);
            void editVarDescription(const std::string& name, const std::string& description);
            void renameVar(const std::string& oldName, const std::string& newName);

            bool havePassword();
            bool truePassword(const std::string& password);
            //bool trueKey(std::string longUserKey);

            std::map<std::string, std::string> getAllVars();
            std::string getVarName();
            std::string getVarDescription();

            std::string getUsername();
            std::string getDisplayName();
            std::string getLanguage();
            permissionsEC getPermissions();

            std::function<void()> varFuncStart(const std::string& name);
    };
}

#endif