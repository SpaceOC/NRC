#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>

extern void CORE_help();
extern void CORE_info_user();
extern void CORE_all_info_users();
extern void CORE_add_user();
extern void CORE_delete_user();
extern void CORE_set_permissions_user();
extern void CORE_info();

class HandlerCommands {
    private:
        std::map<std::string, std::function<void()>> commandMap;
    public:
        HandlerCommands() {
            commandMap["help"] = CORE_help;
            //commandMap["tree"] = CORE_tree;
            commandMap["info_user"] = CORE_info_user;
            commandMap["add_user"] = CORE_add_user;
            //commandMap["delete_user"] = CORE_delete_user;
            commandMap["set_permissions_user"] = CORE_set_permissions_user;
            commandMap["all_info_users"] = CORE_all_info_users;
            commandMap["core_info"] = CORE_info;
        }
        virtual void sendCommand(std::string command) const {
            auto it = commandMap.find(command);
            if (it!= commandMap.end()) {
                it->second();
            }
            else {
                std::cout << "Command not found" << std::endl;
            }
        }
        /*
        void addCommand(std::string Name, std::string Description, std::function<void()> Function) {
            commandMap[Name] = Function;
        }
        */
};