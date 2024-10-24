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
#include "Core/experimental/event_manager.h"

void core::main::addCommands() {
	HandlerCommands::addCommand(
        "help",
        {"shows a list of all commands", {"name"}},
        core::commands::CORE_COMMAND_help,
        0,
        1
    );

	HandlerCommands::addCommand(
        "time",
        {"shows the execution time of a certain command", {"command"}},
        core::commands::CORE_COMMAND_time,
        1,
        999
    );

    HandlerCommands::addCommand(
        "create_user",
        {"creating a new user in the system", {"name", "perms."}},
        core::commands::CORE_COMMAND_createUser,
        1,
        2
    );

    //HandlerCommands::addCommand("exit", "exit", core::commands::CORE_COMMAND_exit);

	HandlerCommands::addCommand(
        "cd",
        {"cd", {"where"}},
        core::commands::CORE_COMMAND_cd,
        1,
        1
    );

	HandlerCommands::addCommand(
        "create_file",
        {"creates a new file", {"full-path"}},
        core::commands::CORE_COMMAND_createFile,
        1,
        1
    );

	HandlerCommands::addCommand(
        "create_link_file",
        {"create a new linked file", {"where, file"}},
        core::commands::CORE_COMMAND_createLinkFile,
        2,
        2
    );

	HandlerCommands::addCommand(
        "delete_file",
        {"deletes the file", {"full-path"}},
        core::commands::CORE_COMMAND_deleteFile,
        1,
        1
    );

	HandlerCommands::addCommand(
        "rename_file",
        {"renames the file", {"full-path", "new-name"}},
        core::commands::CORE_COMMAND_renameFile,
        2,
        2
    );

	HandlerCommands::addCommand(
        "move_file",
        {"moves the file", {"full-path", "new-path"}},
        core::commands::CORE_COMMAND_moveFile,
        2,
        2
    );

	HandlerCommands::addCommand(
        "show_file_data",
        {"show file data", {"full-path"}},
        core::commands::CORE_COMMAND_showFileData,
        1,
        1
    );

	HandlerCommands::addCommand(
        "create_folder",
        {"creates new folder", {"full-path"}},
        core::commands::CORE_COMMAND_createFolder,
        1,
        1
    );

	HandlerCommands::addCommand(
        "delete_folder",
        {"deletes the folder", {"full-path"}},
        core::commands::CORE_COMMAND_deleteFolder,
        1,
        1
    );

	HandlerCommands::addCommand(
        "create_link_folder",
        {"creates a new linked folder", {"where", "folder"}},
        core::commands::CORE_COMMAND_createLinkFolder,
        2,
        2
    );

	HandlerCommands::addCommand(
        "rename_folder",
        {"renames the folder", {"full-path", "new-name"}},
        core::commands::CORE_COMMAND_renameFolder,
        2,
        2
    );

	HandlerCommands::addCommand(
        "move_folder",
        {"moves the folder", {"full-path", "new-path"}},
        core::commands::CORE_COMMAND_moveFolder,
        2,
        2
    );

	HandlerCommands::addCommand(
        "show_folder_data",
        {"show folder data", {"full-path"}},
        core::commands::CORE_COMMAND_showFolderData,
        1,
        1
    );

	HandlerCommands::addCommand(
        "whereim",
        "displays information about the current folder and its full path",
        core::commands::CORE_COMMAND_whereIm
    );
    
	HandlerCommands::addCommand(
        "disk_size",
        "show disk size",
        core::PseudoFS::printDiskSize
    );

	HandlerCommands::addCommand(
        "tree",
        {"shows all folders in a tree view", {"start-path", "include"}},
        core::commands::CORE_COMMAND_tree,
        0,
        2
    );

	HandlerCommands::addCommand(
        "show_all",
        {"shows all files and folders", {"start-path", "include"}},
        core::commands::CORE_COMMAND_showAll,
        0,
        2
    );

	HandlerCommands::addCommand(
        "dir",
        {"shows all files and folders in current directory", {"include"}},
        core::commands::CORE_COMMAND_dir,
        0,
        1
    );

	HandlerCommands::addCommand(
        "search_file",
        {"search files", {"what", "start-path"}},
        core::commands::CORE_COMMAND_searchFile,
        1,
        2
    );

	HandlerCommands::addCommand(
        "edit_display_name",
        {"edit display name", {"name"}},
        core::commands::CORE_COMMAND_editDisplayName,
        1,
        1
    );

    HandlerCommands::addCommand(
        "delete_user",
        {"deleting a user in the system", {"username"}},
        core::commands::CORE_COMMAND_deleteUser,
        1,
        1
    );

	HandlerCommands::addCommand(
        "set_user_permissions",
        {"user permission change", {"name", "perms."}},
        core::commands::CORE_COMMAND_setPermissionsUser,
        2,
        2
    );

    HandlerCommands::addCommand(
        "set_password",
        {"set password", {"old/new", "new"}},
        core::commands::CORE_COMMAND_setPassword,
        1,
        2
    );

	HandlerCommands::addCommand(
        "all_users_info",
        "shows all information about all users",
        core::commands::CORE_COMMAND_allInfoUsers
    );

    HandlerCommands::addCommand(
        "whoim",
        "shows information about the current user",
        core::commands::CORE_COMMAND_whoim
    );

	HandlerCommands::addCommand(
        "user_info",
        {"shows information about the current user", {"username"}},
        core::commands::CORE_COMMAND_infoUser,
        1,
        1
    );

	HandlerCommands::addCommand(
        "rename_user",
        {"renames the user", {"old", "new"}},
        core::commands::CORE_COMMAND_renameUser,
        2,
        2
    );

	HandlerCommands::addCommand(
        "core_info",
        "shows information about the core",
        core::commands::CORE_COMMAND_info
    );

	HandlerCommands::addCommand(
        "logout",
        "logging out of the current user account",
        core::commands::CORE_COMMAND_logout
    );
}

core::main::main() {
    fixNOW();
    addCommands();
    core::UserManager::checkOOBE();
    core::EventManager::enableEvents = true;
    if (!core::UserManager::getOOBEPassed() && core::UserManager::yourUsername().empty()) {
        FileManager FM;
        FM.createFolders({"Data", "Data/Users", "Modules", "Temp"});
        FM.createFiles({"Data/MainData.json", "Data/Users.json", "Data/PFS-Data.json"});
        core::PseudoFS::init();
        core::PseudoFS::createFile("./hello.txt");
        core::PseudoFS::setFileAtt("./hello.txt", "content", "Hello from NRC!");
        core::PseudoFS::savePFS();
        DataManager DM;
        DM.createData("Data/MainData.json", "OOBE_Passed", "0");
        OOBE();
    }
    else {
        core::PseudoFS::loadPFS();
        core::UserManager::userLists();
    }
}

core::main::~main() {
	core::structDataEvents::NRCShutdownEvent eventData = {
		core::UserManager::yourUsername(),
		core::UserManager::currentUserData().getPermissions(),
		static_cast<size_t>(core::UserManager::userVectorPos(core::UserManager::yourUsername()))
	};

	core::EventManager::eventsStart(NRC_SHUTDOWN_EVENT, eventData);

    core::PseudoFS::savePFS();
    std::cout << "Goodbye, " + (core::UserManager::yourUsername() != "" ? core::UserManager::yourUsername() : "user") + ".\n";
}