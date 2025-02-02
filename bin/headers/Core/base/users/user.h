#ifndef NRC_BASE_USERS_USER_H_
#define NRC_BASE_USERS_USER_H_

#include <string>
#include <functional>
#include "Core/settings.h"
#include "permissions_enum_class.h"
#include "Core/base/print_tools.h"
#include "Core/extra/variable_type_enum.h"

namespace core {
    class VariablesManager;
    #ifdef NRC_WEB
    /*
    class PseudoFS;
    */
    #endif
    class UserManager;
    struct VariableData;

    class User : public PrintableClass {
        private:
            bool userCreated = false;
            bool havePasswordV = false;

            friend class UserManager;
            #ifdef NRC_WEB
            /*
            friend class PseudoFS;
            */
            #endif
        protected:
            VariablesManager* vm;
            std::string username, displayName, language, password;
            Permissions permissions;
            #ifdef NRC_WEB
            /*
            std::string curPath;
            size_t curDisk;
            */
            #endif
        public:
            std::string toString() const override {
                return "[User] | { Username - " + username + " | Display Name - " + displayName + " | Language - " + language + " | havePassword - " + (havePasswordV ? "true" : "false") + " }";
            }

            User();
            // User Creation.
            User(const std::string& username, const Permissions& permissions, const std::string& language = "English", const std::string& password = "");
            User(User&);

            // Creating a local (for the user) variable.
            void addVar(const std::string& name, const core::VariableType type, const std::string& c);
            // Creating a local (for the user) variable.
            void addVar(const std::string& name, const core::VariableType type, const std::function<std::string(core::VariableData)>& f);

            void editUsername(const std::string& newUsername);
            void editDisplayName(const std::string& newDisplayName);
            void editLanguage(const std::string& newLanguage);
            void editPassword(const std::string& password);
            void editPermissions(const Permissions& newPermissions);

            // Renaming a variable
            void renameVar(const std::string& oldName, const std::string& newName);

            // @return Returns true if the value of the password variable is not empty.
            bool havePassword();

            // @return Returns true if the password from the argument matches the password from the password variable.
            bool truePassword(const std::string& password);
            //bool trueKey(std::string longUserKey);

            // @return Returns the names and descriptions of all local variables or {} (empty map) if the localVariables variable is empty.
            std::vector<VariableData> getAllVars();


            std::string getUsername();
            std::string getDisplayName();
            std::string getLanguage();
            Permissions getPermissions();

            // Starts a variable function.
            void varFuncStart(std::string_view name);
            // Starts a variable function (and return).
            void varFuncStart(std::string_view name, std::string& str);

            bool varExists(std::string_view name);

            /*
            constexpr bool operator==(const core::Permissions permissions) {
                return (this->permissions == permissions);
            }

            constexpr bool operator==(const std::string& username) {
                return (this->username == username);
            }
            */

            constexpr bool operator==(const User& user) {
                return (this->username == user.username && this->password == user.password && this->permissions == user.permissions);
            }

            constexpr bool operator<(const User& user) {
                return (this->permissions < user.permissions);
            }

            constexpr bool operator>(const User& user) {
                return (this->permissions > user.permissions);
            }
            
    };
}

#endif