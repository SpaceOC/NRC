#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "../../extra/variables.cpp"

extern void CORE_COMMAND_help();
extern void CORE_COMMAND_renameUser();
extern void CORE_COMMAND_allInfoUsers();
extern void CORE_COMMAND_addUser();
extern void CORE_COMMAND_deleteUser();
extern void CORE_COMMAND_setPermissionsUser();
extern void CORE_COMMAND_info();
extern void CORE_COMMAND_logout();
extern void CORE_COMMAND_infoUser();

class HandlerCommands {
    private:
        static inline std::map<std::string, std::function<void()>> commandMap;
        static inline std::map<std::string, std::string> commandMapDescription;
    public:
        HandlerCommands() {
            if (commandMap.size() == 0 && commandMapDescription.size() == 0) {
                addCommand("help", "shows a list of all commands" , CORE_COMMAND_help);
                //addCommand("tree", "shows all files and folders in a tree view" , CORE_COMMAND_tree);
                addCommand("add_user", "creating a new user in the system" , CORE_COMMAND_addUser);
                addCommand("delete_user", "deleting a user in the system" , CORE_COMMAND_deleteUser);
                addCommand("set_user_permissions", "user permission change" , CORE_COMMAND_setPermissionsUser);
                addCommand("all_users_info", "shows all information about all users" , CORE_COMMAND_allInfoUsers);
                addCommand("current_user_info", "shows information about the current user" , CORE_COMMAND_infoUser);
                addCommand("rename_user", "renames the user" , CORE_COMMAND_renameUser);
                addCommand("core_info", "shows information about the core" , CORE_COMMAND_info);
                addCommand("logout", "logging out of the current user account", CORE_COMMAND_logout);
            }
        }

        virtual bool systemVariable(std::string command) const {
            if (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%") {
                return true;
            }
            else {
                return false;
            }
        }

        virtual void parsing(std::string command) const {
        }

        virtual void sendCommand(std::string command) const {
            auto it = commandMap.find(command);
            if (it != commandMap.end()) { it->second(); }
            else if (systemVariable(command)) {
                systemVariables SV; SV.sendVariable(command);
                SV.sendVariable(command);
            }
            else { std::cout << "Command not found" << std::endl; }
        }

        void addCommand(std::string commandName, std::string commandDescription, std::function<void()> commandFunction) const {
            const int maxSpaces = 20;
            std::string Temp;
            int spacesToAdd = std::max(0, maxSpaces - static_cast<int>(commandName.length()));
            Temp += std::string(spacesToAdd, ' ');
            Temp += "\t  " + commandDescription;
            commandMap[commandName] = commandFunction;
            commandMapDescription[commandName] = Temp;
        }
        
        virtual void getAllCommands() const {
            std::vector<std::string> commandVectorName, commandVectorDescription;
            for (auto elements : commandMap) { commandVectorName.push_back(elements.first); }
            for (auto elements : commandMapDescription) { commandVectorDescription.push_back(elements.second); }
            for (int i = 0; (commandVectorName.size() - 1) >= i && (commandVectorDescription.size() - 1) >= i; i++) {
                std::cout << commandVectorName[i] + commandVectorDescription[i] << '\n';
            }
        }
};