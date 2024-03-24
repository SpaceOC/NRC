#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "../../extra/variables.cpp"

extern void CORE_help();
extern void CORE_info_user();
extern void CORE_rename_user();
extern void CORE_all_info_users();
extern void CORE_add_user();
extern void CORE_delete_user();
extern void CORE_set_permissions_user();
extern void CORE_info();

class HandlerCommands {
    private:
        static inline std::map<std::string, std::function<void()>> commandMap;
        static inline std::map<std::string, std::string> commandMapDescription;
    public:
        HandlerCommands() {
            if (commandMap.size() == 0 && commandMapDescription.size() == 0) {
                addCommand("help", "shows a list of all commands" , CORE_help);
                //addCommand("tree", "shows all files and folders in a tree view" , CORE_tree);
                addCommand("add_user", "creating a new user in the system" , CORE_add_user);
                addCommand("delete_user", "deleting a user in the system" , CORE_delete_user);
                addCommand("set_user_permissions", "user permission change" , CORE_set_permissions_user);
                addCommand("all_users_info", "shows all information about all users" , CORE_all_info_users);
                addCommand("current_user_info", "shows information about the current user" , CORE_info_user);
                addCommand("rename_user", "renames the user" , CORE_rename_user);
                addCommand("core_info", "shows information about the core" , CORE_info);
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