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
#include <string>
#include "Core/main.h"
#include "Core/base/users/OOBE.h"
#include "Core/base/filesystem/pseudo_fs.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/data/file_manager.h"
#include "Core/base/data/data_manager.h"
#include "Core/base/command/handler_commands.h"
#include "Core/base/command/commands.h"

void core::main::addCommands() {
	handlerCommands::addCommand("help", {"shows a list of all commands", {"name"}}, core::commands::CORE_COMMAND_help, 0, 1);
	handlerCommands::addCommand("time", {"shows the execution time of a certain command", {"command"}}, core::commands::CORE_COMMAND_time, 1, 999);
    //addCommand("exit", "exit", core::commands::CORE_COMMAND_exit);
	//addCommand("cd", "cd" , core::commands::CORE_COMMAND_cd);
	//addCommand("tree", "shows all files and folders in a tree view" , core::commands::CORE_COMMAND_tree);
	handlerCommands::addCommand("add_user", "creating a new user in the system", core::commands::CORE_COMMAND_addUser);
	handlerCommands::addCommand("delete_user", "deleting a user in the system", core::commands::CORE_COMMAND_deleteUser);
	handlerCommands::addCommand("set_user_permissions", "user permission change", core::commands::CORE_COMMAND_setPermissionsUser);
	handlerCommands::addCommand("all_users_info", "shows all information about all users", core::commands::CORE_COMMAND_allInfoUsers);
	handlerCommands::addCommand("whoim", "shows information about the current user", core::commands::CORE_COMMAND_infoUser);
	handlerCommands::addCommand("rename_user", "renames the user", core::commands::CORE_COMMAND_renameUser);
	handlerCommands::addCommand("core_info", "shows information about the core", core::commands::CORE_COMMAND_info);
	handlerCommands::addCommand("logout", "logging out of the current user account", core::commands::CORE_COMMAND_logout);
}

core::main::main() {
    fixNOW();
    addCommands();
    userManager UM;
    UM.checkOOBE_Passed();
    if (!UM.getOOBE_Passed() && UM.yourUsername().empty()) {
        fileManager FM;
        FM.createFolders({"Data", "Data/Users", "Modules", "FS", "Temp"});
        FM.createFiles({"Data/MainData.json", "Data/Users.json"});
        dataManager DM;
        DM.createData("Data/MainData.json", "OOBE_Passed", "0");
        OOBE();
    }
    else
        UM.userLists();
}

core::main::~main() {
    userManager UM;
    std::cout << "Goodbye, " + (UM.yourUsername() != "" ? UM.yourUsername() : "user") + ".\n";
}