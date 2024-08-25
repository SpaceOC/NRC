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
	handlerCommands::addCommand("help", "shows a list of all commands", core::commands::CORE_COMMAND_help);
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