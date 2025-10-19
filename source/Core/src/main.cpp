#include <string>
#include <fstream>
#include "dylib.hpp"
#include "Core/main.h"
#include "Core/users/OOBE.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/users/user_manager.h"
#include "Core/command/commands_handler.h"
#include "Core/command/command_parser.h"
#include "Core/command/command_structs.h"
#include "Core/command/commands.h"
#include "Core/users/user.h"
#include "Core/experimental/event_manager.h"
#include "Core/print/print.h"
#include "Core/other/variables.h"
#include "Core/command/command_sender.h"
#include "Core/modules/module_base.h"
#include "Core/experimental/thread_manager.h"
#include "Core/modules/module_metadata.h"
#include "Core/utils/other_util.h"

#include "Core/experimental/run_js_code.h"

void core::main::addCommands() {
	commandsHandler()->addCommand(
		"help",
		{"shows a list of all commands", {"name"}},
		&(core::commands::CORE_COMMAND_help),
		0,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"time",
		{"shows the execution time of a certain command", {"command"}},
		&(core::commands::CORE_COMMAND_time),
		1,
		999,
		{}
	);
	
	commandsHandler()->addCommand(
		"cd",
		{"cd", {"where"}},
		&(core::commands::CORE_COMMAND_cd),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"create_file",
		{"creates a new file", {"full-path"}},
		&(core::commands::CORE_COMMAND_createFile),
		1,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"create_link_file",
		{"create a new linked file", {"target-file", "where-create"}},
		&(core::commands::CORE_COMMAND_createLinkFile),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"delete_file",
		{"deletes the file", {"full-path"}},
		&(core::commands::CORE_COMMAND_deleteFile),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"rename_file",
		{"renames the file", {"full-path", "new-name"}},
		&(core::commands::CORE_COMMAND_renameFile),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"move_file",
		{"moves the file", {"full-path", "new-path"}},
		&(core::commands::CORE_COMMAND_moveFile),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"file_data",
		{"show file data", {"full-path"}},
		&(core::commands::CORE_COMMAND_showFileData),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"read_file",
		{"read file", {"full-path"}},
		&(core::commands::CORE_COMMAND_readFile),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"create_folder",
		{"creates new folder", {"full-path"}},
		&(core::commands::CORE_COMMAND_createFolder),
		1,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"delete_folder",
		{"deletes the folder", {"full-path"}},
		&(core::commands::CORE_COMMAND_deleteFolder),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"create_link_folder",
		{"creates a new linked folder", {"target-folder", "where-create"}},
		&(core::commands::CORE_COMMAND_createLinkFolder),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"rename_folder",
		{"renames the folder", {"full-path", "new-name"}},
		&(core::commands::CORE_COMMAND_renameFolder),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"move_folder",
		{"moves the folder", {"full-path", "new-path"}},
		&(core::commands::CORE_COMMAND_moveFolder),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"folder_data",
		{"show folder data", {"full-path"}},
		&(core::commands::CORE_COMMAND_showFolderData),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"whereim",
		"displays information about the current folder and its full path",
		&(core::commands::CORE_COMMAND_whereIm)
	);
	
	commandsHandler()->addCommand(
		"disk_size",
		"show disk size",
		&(core::commands::CORE_COMMAND_printDiskSize)
	);

	commandsHandler()->addCommand(
		"tree",
		{"shows all folders in a tree view", {"start-path", "include"}},
		&(core::commands::CORE_COMMAND_tree),
		0,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"show_all",
		{"shows all files and folders", {"start-path", "include"}},
		&(core::commands::CORE_COMMAND_showAll),
		0,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"dir",
		{"shows all files and folders in current directory", {"include"}},
		&(core::commands::CORE_COMMAND_dir),
		0,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"search_file",
		{"search files", {"what", "start-path"}},
		&(core::commands::CORE_COMMAND_searchFile),
		1,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"edit_display_name",
		{"edit display name", {"name"}},
		&(core::commands::CORE_COMMAND_editDisplayName),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"set_password",
		{"set password", {"old/new", "new"}},
		&(core::commands::CORE_COMMAND_setPassword),
		1,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"all_users_info",
		"shows all information about all users",
		&(core::commands::CORE_COMMAND_allInfoUsers)
	);

	commandsHandler()->addCommand(
		"user_info",
		{"shows information about the current user", {"username"}},
		&(core::commands::CORE_COMMAND_infoUser),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"whoim",
		"shows information about the current user",
		&(core::commands::CORE_COMMAND_whoim)
	);

	#ifndef NRC_WEB
	commandsHandler()->addCommand(
		"rename_user",
		{"renames the user", {"old", "new"}},
		&(core::commands::CORE_COMMAND_renameUser),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"delete_user",
		{"deleting a user in the system", {"username"}},
		&(core::commands::CORE_COMMAND_deleteUser),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"set_user_permissions",
		{"user permission change", {"name", "perms."}},
		&(core::commands::CORE_COMMAND_setPermissionsUser),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"create_user",
		{"creating a new user in the system", {"name", "perms."}},
		&(core::commands::CORE_COMMAND_createUser),
		1,
		2,
		{}
	);
	#endif

	commandsHandler()->addCommand(
		"core_info",
		"shows information about the core",
		&(core::commands::CORE_COMMAND_info)
	);

	commandsHandler()->addCommand(
		"logout",
		"logging out of the current user account",
		&(core::commands::CORE_COMMAND_logout)
	);

	commandsHandler()->addCommand(
		"edit_file",
		{"edit file", {"full-path", "content"}},
		&(core::commands::CORE_COMMAND_editFile),
		1,
		2,
		{2}
	);

	commandsHandler()->addCommand(
		"wrl",
		{"write content in new line", {"file-path", "text"}},
		&(core::commands::CORE_COMMAND_writeOnNewLineFile),
		2,
		3,
		{2}
	);

	commandsHandler()->addCommand(
		"wr",
		{"write content in end file", {"file-path", "text"}},
		&(core::commands::CORE_COMMAND_writeFile),
		2,
		3,
		{2}
	);

	commandsHandler()->addCommand(
		"rewr",
		{"rewrite all content in file", {"file-path", "text"}},
		&(core::commands::CORE_COMMAND_rewriteFile),
		2,
		3,
		{2}
	);

	commandsHandler()->addCommand(
		"fileclr",
		{"clear all content in file", {"file-path"}},
		&(core::commands::CORE_COMMAND_rewriteFile),
		1,
		1,
		{}
	);

	commandsHandler()->addCommand(
		"set_file_owner",
		{"set new file owner (if arg1 == '[ NONE ]' - none)", {"file-path", "new-owner"}},
		&(core::commands::CORE_COMMAND_setNewFileOwner),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"set_folder_owner",
		{"set new folder owner (if arg1 == '[ NONE ]' - none)", {"file-path", "new-owner"}},
		&(core::commands::CORE_COMMAND_setNewFolderOwner),
		2,
		2,
		{}
	);

	commandsHandler()->addCommand(
		"set",
		{"set new value", {"name", "type", "value"}},
		&(core::commands::CORE_COMMAND_addLocalVar),
		3,
		3,
		{3}
	);

	commandsHandler()->addCommand(
		"vars",
		"show all local vars",
		&(core::commands::CORE_COMMAND_allLocalVars)
	);
}

void core::main::addCRules() {
	// Adding a check that the command name is a call to an environment variable
	commandsHandler()->addCustomRules("variable", [](const core::CommandObject& c, core::User* who, std::string& ret, std::string&) -> bool {
		if (!commandsHandler()->thisVariable(c.name))
			return false;
		std::string varName = c.name.substr(1, c.name.length() - 2);
		if (who->varExists(varName))
			who->varFuncStart(varName, ret);
		else if (core::systemVariablesManager()->exists(varName))
			core::systemVariablesManager()->start(c.name, ret);
		return true;
	});

	// Adding a check that this is a call to a .clf file
	commandsHandler()->addCustomRules(".clf", [](const core::CommandObject& c, core::User* who, std::string& ret, std::string& err) -> bool {
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
			std::vector<core::CommandObject> parsedCommands = core::commandsHandler()->getParser()->parse(uir);
			for (core::CommandObject& command : parsedCommands) {
				#ifndef NRC_WEB
				command.returnable = false;
				core::commandsHandler()->sendCommand(who, command);
				#else
				std::string str;
				core::commandsHandler()->sendCommand(command, str);
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

#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES

// TODO: Добавить конфигурационный файл modules.json, его загрузку и сохранение
void core::main::searchModules() {
	for (auto& it : std::filesystem::recursive_directory_iterator("./Modules")) {
		if (it.is_directory()) {
			std::string name = it.path().filename().generic_string();
			checkModule(name);
		}
	}
}

void core::main::checkModule(const std::string& name) {
	std::string modulePath = "./Modules/" + name + "/";
	if (!std::filesystem::exists(modulePath + "lib.json")) {
		std::cout << "lib.json not found!\n";
		return;
	}
	else if (!std::filesystem::exists(modulePath + "translations.json")) {
		std::cout << "translations.json not found!\n";
		return;
	}
	else if (!std::filesystem::exists(modulePath + "bin")) {
		std::cout << "bin not found!\n";
		return;
	}
	else if (!std::filesystem::exists(modulePath + "langs")) {
		std::cout << "langs not found!\n";
		return;
	}

	ModuleMetadata moduleMetadata;
	nlohmann::json j = nlohmann::json::parse(other_util::getFileContent(modulePath + "lib.json"));
	moduleMetadata.makeMetadataFromJSON(j);
	if (moduleMetadata.requiredVersionInRange && (*(moduleMetadata.uses[0]) > version && *(moduleMetadata.uses[1]) < version)) {
		std::string requiresVersionMessage = 
			(
				moduleMetadata.uses[0]->getVersionStr() != moduleMetadata.uses[1]->getVersionStr() ?
				moduleMetadata.uses[0]->getVersionStr() + "-" + moduleMetadata.uses[1]->getVersionStr() :
				moduleMetadata.uses[0]->getVersionStr()
			);
		
		core::print(core::PrintColors::red, "'", name, "' can't be loaded!\n",
			"This module requires NRC version " + requiresVersionMessage
		);
	}

	try {
	  	std::string binName = moduleMetadata.layout[OS_NAME_STR];
	  	dylib::library moduleRaw(modulePath + "bin/" + (binName.empty() ? OS_NAME_STR : binName));
	  	auto createModuleFunction = moduleRaw.get_function<ModuleBase *(ModuleMetadata *, main *)>("createModule");
	  	auto moduleb = createModuleFunction(&moduleMetadata, this);
	  	if (moduleb) {
			//std::unique_ptr<ModuleBase> moduleInUniquePtr(std::move(moduleb));
			modules.push_back(std::move(moduleb));
	  	}
	}
	catch (std::exception &e) {
	  	std::cout << e.what() << '\n';
	}
}

void nothing() {}

void* core::main::getRequiredClassPtr(const std::string& request, ModuleBase*) {
	/*
	if (modules.empty())
		return 0;
	
	bool moduleFound = std::any_of(modules.begin(), modules.end(), [&module](ModuleBase* f) { return f == module; });

	if (!moduleFound)
		return 0;
		*/
	
	if (request == "PseudoFS") {
		return (void*)pseudoFS();
	}
	else if (request == "UserManager") {
		return (void*)userManager();
	}
	else if (request == "CommandsHandler") {
		return (void*)commandsHandler();
	}
	else if (request == "SystemVariablesManager") {
		return (void*)systemVariablesManager();
	}
	return (void*)(&nothing);
}

#endif

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
	commandSenderReplaced = true;
}

core::main::main() {
	loopedFunc = [this]() -> void {
		#ifndef NRC_WEB
		commandSender->zone();
		#endif
	};

	startFunc = []() -> void {
		core::print("Welcome to NRC!\n");
	};
}

core::main::main(std::function<void()> start, std::function<void()> loop) {
	startFunc = std::move(start);
	loopedFunc = std::move(loop);
}

core::main::main(std::function<void()> start) {
	startFunc = std::move(start);
	loopedFunc = [this]() -> void {
		#ifndef NRC_WEB
		commandSender->zone();
		#endif
	};
}

void core::main::init() {
	fixNOW();
	addCommands();
	addCRules();

	if (!std::filesystem::exists("Data"))
		std::filesystem::create_directory("Data");
	if (!std::filesystem::exists("Data/Users"))
		std::filesystem::create_directory("Data/Users");
	if (!std::filesystem::exists("Modules"))
		std::filesystem::create_directory("Modules");
	if (!std::filesystem::exists("Temp"))
		std::filesystem::create_directory("Temp");

	#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
	searchModules();
	#endif

	if (!std::filesystem::exists("Data/main.json")) {
		std::ofstream data("Data/main.json", std::ios::out);
		nlohmann::json j;
		j["OOBE_Passed"] = false;
		data << j.dump(2);
		data.close();
	}

	#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
	core::experimental::EventManager::enableEvents = true;
	#endif

	core::pseudoFS()->init();
	core::userManager()->readAllUsersData();
	core::pseudoFS()->postInit();

	if (!commandSenderReplaced) {
		commandSender = new CommandSenderBasic();
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
	#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
	core::experimental::structDataEvents::NRCShutdownEvent eventData = {
		core::userManager()->yourUsername(),
		core::userManager()->currentUserData().getPermissions(),
		static_cast<size_t>(core::userManager()->userVectorPos(core::userManager()->yourUsername()))
	};

	core::experimental::EventManager::eventsStart(NRC_SHUTDOWN_EVENT, eventData);
	#endif

	core::pseudoFS()->getNRFS()->saveData();
	std::cout << "Goodbye, " + (core::userManager()->yourUsername() != "" ? core::userManager()->yourUsername() : "user") + ".\n";
}