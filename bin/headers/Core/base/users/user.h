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
#ifndef NRC_BASE_USERS_USER_H_
#define NRC_BASE_USERS_USER_H_

#include <string>
#include "Core/extra/variables.h"
#include "permissions_enum_class.h"

namespace core {
    class User {
        private:
            std::map<std::string, VariableData> localVariables;
            std::string username, displayName, language, password;
            permissionsEC permissions;
            bool userCreated = false;
        public:

            User();
            // User Creation.
            User(const std::string& username, const permissionsEC& permissions, const std::string& language = "English", const std::string& password = "");
            
            // Creating a local (for the user) variable.
            void addVar(const std::string& name, const std::string& description, const std::function<void()>& varFunction);

            void editUsername(const std::string& newUsername);
            void editDisplayName(const std::string& newDisplayName);
            void editLanguage(const std::string& newLanguage);
            void editPassword(const std::string& password);
            void editPermissions(const permissionsEC& newPermissions);

            // Changes the function of a variable.
            void editVarFunction(const std::string& name, const std::function<void()>& function);
            // Modifies the description of the variable.
            void editVarDescription(const std::string& name, const std::string& description);
            // Renaming a variable
            void renameVar(const std::string& oldName, const std::string& newName);

            // @return Returns true if the value of the password variable is not empty.
            bool havePassword();

            // @return Returns true if the password from the argument matches the password from the password variable.
            bool truePassword(const std::string& password);
            //bool trueKey(std::string longUserKey);

            // @return Returns the names and descriptions of all local variables or {} (empty map) if the localVariables variable is empty.
            std::map<std::string, std::string> getAllVars();

            // @return Returns the variable description or an empty value if the localVariables variable is empty.
            std::string getVarDescription(std::string name);

            std::string getUsername();
            std::string getDisplayName();
            std::string getLanguage();
            std::string getPassword();
            permissionsEC getPermissions();

            // Starts a variable function.
            void varFuncStart(const std::string& name);
    };
}

#endif