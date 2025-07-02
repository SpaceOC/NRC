#include <string>
#include <fstream>
#include "Core/main.h"
#include "Core/users/OOBE.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/users/user_manager.h"
#include "Core/command/handler_commands.h"
#include "Core/command/command_parser.h"
#include "Core/command/command_structs.h"
#include "Core/command/commands.h"
#include "Core/users/user.h"
#include "Core/experimental/event_manager.h"
#include "Core/print/print.h"
#include "Core/other/variables.h"
#include "Core/command/command_sender.h"
#include "Core/experimental/process.h"

#include "Core/experimental/run_js_code.h"

void core::main::addCommands() {
	handlerCommands()->addCommand(
		"help",
		{"shows a list of all commands", {"name"}},
		core::commands::CORE_COMMAND_help,
		0,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"time",
		{"shows the execution time of a certain command", {"command"}},
		core::commands::CORE_COMMAND_time,
		1,
		999,
		{}
	);

	//handlerCommands()->addCommand("exit", "exit", core::commands::CORE_COMMAND_exit);

	handlerCommands()->addCommand(
		"cd",
		{"cd", {"where"}},
		core::commands::CORE_COMMAND_cd,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"create_file",
		{"creates a new file", {"full-path"}},
		core::commands::CORE_COMMAND_createFile,
		1,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"create_link_file",
		{"create a new linked file", {"target-file", "where-create"}},
		core::commands::CORE_COMMAND_createLinkFile,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"delete_file",
		{"deletes the file", {"full-path"}},
		core::commands::CORE_COMMAND_deleteFile,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"rename_file",
		{"renames the file", {"full-path", "new-name"}},
		core::commands::CORE_COMMAND_renameFile,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"move_file",
		{"moves the file", {"full-path", "new-path"}},
		core::commands::CORE_COMMAND_moveFile,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"file_data",
		{"show file data", {"full-path"}},
		core::commands::CORE_COMMAND_showFileData,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"read_file",
		{"read file", {"full-path"}},
		core::commands::CORE_COMMAND_readFile,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"create_folder",
		{"creates new folder", {"full-path"}},
		core::commands::CORE_COMMAND_createFolder,
		1,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"delete_folder",
		{"deletes the folder", {"full-path"}},
		core::commands::CORE_COMMAND_deleteFolder,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"create_link_folder",
		{"creates a new linked folder", {"target-folder", "where-create"}},
		core::commands::CORE_COMMAND_createLinkFolder,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"rename_folder",
		{"renames the folder", {"full-path", "new-name"}},
		core::commands::CORE_COMMAND_renameFolder,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"move_folder",
		{"moves the folder", {"full-path", "new-path"}},
		core::commands::CORE_COMMAND_moveFolder,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"folder_data",
		{"show folder data", {"full-path"}},
		core::commands::CORE_COMMAND_showFolderData,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"whereim",
		"displays information about the current folder and its full path",
		core::commands::CORE_COMMAND_whereIm
	);
	
	handlerCommands()->addCommand(
		"disk_size",
		"show disk size",
		core::commands::CORE_COMMAND_printDiskSize
	);

	handlerCommands()->addCommand(
		"tree",
		{"shows all folders in a tree view", {"start-path", "include"}},
		core::commands::CORE_COMMAND_tree,
		0,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"show_all",
		{"shows all files and folders", {"start-path", "include"}},
		core::commands::CORE_COMMAND_showAll,
		0,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"dir",
		{"shows all files and folders in current directory", {"include"}},
		core::commands::CORE_COMMAND_dir,
		0,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"search_file",
		{"search files", {"what", "start-path"}},
		core::commands::CORE_COMMAND_searchFile,
		1,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"edit_display_name",
		{"edit display name", {"name"}},
		core::commands::CORE_COMMAND_editDisplayName,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"set_password",
		{"set password", {"old/new", "new"}},
		core::commands::CORE_COMMAND_setPassword,
		1,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"all_users_info",
		"shows all information about all users",
		core::commands::CORE_COMMAND_allInfoUsers
	);

	handlerCommands()->addCommand(
		"user_info",
		{"shows information about the current user", {"username"}},
		core::commands::CORE_COMMAND_infoUser,
		1,
		1,
		{}
	);

	#ifndef NRC_WEB
	handlerCommands()->addCommand(
		"whoim",
		"shows information about the current user",
		core::commands::CORE_COMMAND_whoim
	);

	handlerCommands()->addCommand(
		"rename_user",
		{"renames the user", {"old", "new"}},
		core::commands::CORE_COMMAND_renameUser,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"delete_user",
		{"deleting a user in the system", {"username"}},
		core::commands::CORE_COMMAND_deleteUser,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"set_user_permissions",
		{"user permission change", {"name", "perms."}},
		core::commands::CORE_COMMAND_setPermissionsUser,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"create_user",
		{"creating a new user in the system", {"name", "perms."}},
		core::commands::CORE_COMMAND_createUser,
		1,
		2,
		{}
	);
	#endif

	#ifdef NRC_WEB
	handlerCommands()->addCommand(
		"whoim",
		"shows information about the current user",
		core::commands::CORE_COMMAND_whoim
	);
	#endif

	handlerCommands()->addCommand(
		"core_info",
		"shows information about the core",
		core::commands::CORE_COMMAND_info
	);

	handlerCommands()->addCommand(
		"logout",
		"logging out of the current user account",
		core::commands::CORE_COMMAND_logout
	);

	handlerCommands()->addCommand(
		"edit_file",
		{"edit file", {"full-path", "content"}},
		core::commands::CORE_COMMAND_editFile,
		1,
		2,
		{2}
	);

	handlerCommands()->addCommand(
		"wrl",
		{"write content in new line", {"file-path", "text"}},
		core::commands::CORE_COMMAND_writeOnNewLineFile,
		2,
		3,
		{2}
	);

	handlerCommands()->addCommand(
		"wr",
		{"write content in end file", {"file-path", "text"}},
		core::commands::CORE_COMMAND_writeFile,
		2,
		3,
		{2}
	);

	handlerCommands()->addCommand(
		"rewr",
		{"rewrite all content in file", {"file-path", "text"}},
		core::commands::CORE_COMMAND_rewriteFile,
		2,
		3,
		{2}
	);

	handlerCommands()->addCommand(
		"fileclr",
		{"clear all content in file", {"file-path"}},
		core::commands::CORE_COMMAND_rewriteFile,
		1,
		1,
		{}
	);

	handlerCommands()->addCommand(
		"set_file_owner",
		{"set new file owner (if arg1 == '[ NONE ]' - none)", {"file-path", "new-owner"}},
		core::commands::CORE_COMMAND_setNewFileOwner,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"set_folder_owner",
		{"set new folder owner (if arg1 == '[ NONE ]' - none)", {"file-path", "new-owner"}},
		core::commands::CORE_COMMAND_setNewFolderOwner,
		2,
		2,
		{}
	);

	handlerCommands()->addCommand(
		"set",
		{"set new value", {"name", "type", "value"}},
		core::commands::CORE_COMMAND_addLocalVar,
		3,
		3,
		{3}
	);

	handlerCommands()->addCommand(
		"vars",
		"show all local vars",
		core::commands::CORE_COMMAND_allLocalVars
	);
}

void core::main::addCRules() {
	// Adding a check that the command name is a call to an environment variable
	handlerCommands()->addCustomRules([](const core::CommandObject& c, core::User* who, std::string& ret, std::string&) -> bool {
		if (!handlerCommands()->thisVariable(c.name))
			return false;
		std::string varName = c.name.substr(1, c.name.length() - 2);
		if (who->varExists(varName))
			who->varFuncStart(varName, ret);
		else if (core::systemVariablesManager()->exists(varName))
			core::systemVariablesManager()->start(c.name, ret);
		return true;
	});

	// Adding a check that this is a call to a .clf file
	handlerCommands()->addCustomRules([](const core::CommandObject& c, core::User* who, std::string& ret, std::string& err) -> bool {
		if (!c.name._Starts_with("./"))
			return false;
		else if (!core::string_util::endsWith(c.name, ".clf")) {
			err = "Error: This is NOT .clf file";
			return false;
		}

		int code;
		size_t curDisk = core::pseudoFS()->getCurDiskId();
		std::string commandLinesFileCode = core::pseudoFS()->getFileData(
			core::pseudoFS()->getCurrentPath() + c.name.substr(2, c.name.length()),
			curDisk,
			code
		).content;
		if (code != core::PseudoFSCodes::OK) {
			err = "Error: " + core::pseudoFSCodesS(code);
			return false;
		}
		std::vector<std::string> clfc = core::string_util::split(commandLinesFileCode, '\n');
		#ifdef NRC_WEB
		std::string output;
		#endif
		for (const std::string& uir : clfc) {
			std::vector<core::CommandObject> parsedCommands = core::handlerCommands()->getParser()->parse(uir);
			for (core::CommandObject& command : parsedCommands) {
				#ifndef NRC_WEB
				command.returnable = false;
				core::handlerCommands()->sendCommand(who, command);
				#else
				std::string str;
				core::handlerCommands()->sendCommand(command, str);
				output += "\n" + str;
				#endif
			}
		}
		#ifdef NRC_WEB
		ret = output;
		#else
		ret = "";
		#endif
		return true;
	});
}

void core::main::loop() {
	while (work) {
		loopedFunc();
	}
};

void core::main::stopWork() {
	work = false;
}

void core::main::setCommandSender(CommandSenderBasic* newCommandSender) {
	if (!newCommandSender)
		return;
	commandSender = newCommandSender;
}

core::main::main() {
	auto func = [this]() -> void {
		#ifndef NRC_WEB
		commandSender->zone();
		#endif
	};
	loopedFunc = func;

	auto start = []() -> void {
		core::print("Welcome to NRC!\n");
	};
	startFunc = start;
}

core::main::main(std::function<void()> start, std::function<void()> loop) {
	startFunc = start;
	loopedFunc = loop;
}

core::main::main(std::function<void()> start) {
	startFunc = start;
	auto func = [this]() -> void {
		#ifndef NRC_WEB
		commandSender->zone();
		#endif
	};
	loopedFunc = func;
}

void core::main::init() {
	fixNOW();
	addCommands();
	addCRules();
	core::EventManager::enableEvents = true;
	core::pseudoFS()->init();
	core::userManager()->readAllUsersData();
	core::pseudoFS()->postInit();

	if (!std::filesystem::exists("Data"))
		std::filesystem::create_directory("Data");
	else if (!std::filesystem::exists("Data/Users"))
		std::filesystem::create_directory("Data/Users");
	else if (!std::filesystem::exists("Modules"))
		std::filesystem::create_directory("Modules");
	else if (!std::filesystem::exists("Temp"))
		std::filesystem::create_directory("Temp");

	if (!std::filesystem::exists("Data/main.json")) {
		std::ofstream data("Data/main.json", std::ios::out);
		nlohmann::json j;
		j["OOBE_Passed"] = false;
		data << j.dump(2);
		data.close();
	}
}

void core::main::start() {
	#ifndef NRC_WEB
	core::userManager()->checkOOBE();
	if (!core::userManager()->getOOBEPassed() && core::userManager()->yourUsername().empty())      
		OOBE();
	else
		core::userManager()->userLists();
	#endif
	
	startFunc();
	loop();
};

core::main::~main() {
	core::structDataEvents::NRCShutdownEvent eventData = {
		core::userManager()->yourUsername(),
		core::userManager()->currentUserData().getPermissions(),
		static_cast<size_t>(core::userManager()->userVectorPos(core::userManager()->yourUsername()))
	};

	core::EventManager::eventsStart(NRC_SHUTDOWN_EVENT, eventData);

	core::pseudoFS()->getNRFS()->saveData();
	std::cout << "Goodbye, " + (core::userManager()->yourUsername() != "" ? core::userManager()->yourUsername() : "user") + ".\n";
}