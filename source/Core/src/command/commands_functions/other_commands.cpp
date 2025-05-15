#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <chrono>
#include "Core/print/gprint.h"
#include "Core/command/commands.h"
#include "Core/command/handler_commands.h"
#include "Core/command/command_structs.h"
#include "Core/CORE_info.h"
#include "Core/users/user_manager.h"
#include "Core/users/user.h"
#include "Core/filesystem/pseudo_fs.h"
#include "Core/filesystem/nrfs.h"
#include "Core/other/variables.h"

std::string core::commands::CORE_COMMAND_help(core::User*, core::CommandObject* thisObj) {
	std::string result;
	if (thisObj->args.empty()) {
		for (const auto& command : core::handlerCommands()->getAllCommands()) {
			std::string argsNames = "";
			if (!command.second.argsNames.empty()) {
				for (auto arg : command.second.argsNames) {
					argsNames += " <" + arg + ">";
				}
			}
			result += core::gprint(command.first + argsNames + command.second.description + '\n', PrintColors::light_green);
		}
	}
	else {
		if (core::handlerCommands()->getCommand(thisObj->args.at(0)).empty()) {
			return gprint("Command not found\n", PrintColors::red);
		}
		else {
			std::map<std::string, core::CommandDescription> temp = core::handlerCommands()->getCommand(thisObj->args.at(0));
			std::string argsNames = "";
			if (!temp[thisObj->args.at(0)].argsNames.empty()) {
				for (auto arg : temp[thisObj->args.at(0)].argsNames) {
					argsNames += " <" + arg + ">";
				}
			}
			result = gprint(thisObj->args.at(0) + argsNames + temp[thisObj->args.at(0)].description + '\n', PrintColors::light_green);
		}
	}

	return result;
};

std::string core::commands::CORE_COMMAND_info(core::User*, core::CommandObject*) {
	std::string result = gprint(core::PrintColors::white,
		CORE_NAME, "by", CORE_DEVELOPER, "\n",
		CORE_VERSION, " ", CORE_VERSION_TYPE, "\n",
		"NRFS (Not Real FileSystem) by SpaceOC / NRFS Version - ", NRFS_VERSION, " ", NRFS_VERSION_TYPE, "\n",
		"JSON library by nlohmann - https://github.com/nlohmann/json \n",
		" --- Special Thanks ---\n",
		"Alone Knight - migrating NRC from Makefile to CMake\n"
	);
	/*
	std::string result = CORE_NAME + std::string(" by ") + CORE_DEVELOPER + "\n" + CORE_VERSION + std::string(" ") + CORE_VERSION_TYPE + "\n";
	result += "NRFS (Not Real FileSystem) by SpaceOC\n" + std::string(NRFS_VERSION) + " " + NRFS_VERSION_TYPE + "\n";
	result += "JSON library by nlohmann - https://github.com/nlohmann/json \n";
	result += " --- Special Thanks ---\nAlone Knight - migrating NRC from Makefile to CMake\n";
	*/
	return result;

};

std::string core::commands::CORE_COMMAND_time(core::User* who, core::CommandObject* thisObj) {
	auto start = std::chrono::steady_clock::now();
	std::string temp;

	for (const std::string& anotherTemp : thisObj->args) {
		if (anotherTemp != thisObj->args.at(0))
			temp += (anotherTemp == core::handlerCommands()->getCommandSeparator() ? "" : "\"") + anotherTemp + (anotherTemp != thisObj->args.back() ? (anotherTemp == core::handlerCommands()->getCommandSeparator() ? "" : "\" ") : (anotherTemp == core::handlerCommands()->getCommandSeparator() ? "" : "\""));
		else
			temp += anotherTemp + " ";
	}
	core::CommandObject command = core::handlerCommands()->getParser()->parse(temp).at(0);
	std::string result;

	if (!thisObj->returnable)
		core::handlerCommands()->sendCommand(who, command);
	else
		core::handlerCommands()->sendCommand(who, command, result);

	auto end = std::chrono::steady_clock::now();
	long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::string timeResult = gprint(core::PrintColors::light_green, "time: ", duration, "ms\n");
	result += "\n" + timeResult;

	return result + "\n";
}

std::string core::commands::CORE_COMMAND_addLocalVar(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.empty() || thisObj->args.size() < 3) {
		return core::gprint("Error: Not enough arguments.\n", core::PrintColors::red);
	}

	if (thisObj->args.at(1) != "func") {
		int code;
		size_t curDisk = core::pseudoFS()->getCurDiskId();
		who->addVar(
			thisObj->args.at(0),
			(thisObj->args.at(1) == "command" ? core::VariableType::COMMAND : (thisObj->args.at(1) == "name" ? core::VariableType::NAME : core::VariableType::JS_CODE)),
			(thisObj->args.at(1) == "command" || thisObj->args.at(1) == "name" ? thisObj->args.at(2) : core::pseudoFS()->getFileData(thisObj->args.at(2), curDisk, code).content)
		);
		if (core::userManager()->userExist(who->getUsername()))
			core::userManager()->saveUserData(who->getUsername());
	}
	else {
		return core::gprint("Not ready.\n", core::PrintColors::yellow);
	}

	return "";
}

std::string core::commands::CORE_COMMAND_renameLocalVar(core::User* who, core::CommandObject* thisObj) {
	if (thisObj->args.empty() || thisObj->args.size() < 2) {
		return core::gprint("Error: Not enough arguments.\n", core::PrintColors::red);
	}

	who->renameVar(thisObj->args.at(0), thisObj->args.at(1));
	if (core::userManager()->userExist(who->getUsername()))
		core::userManager()->saveUserData(who->getUsername());

	return "";
}


//   -------------- Other ---------------

std::string core::commands::CORE_COMMAND_allSystemVars(core::User*, core::CommandObject*) {
	std::string result;
	for (auto var : core::systemVariablesManager()->getAllVars()) {
		result += core::gprint(var, core::PrintColors::light_yellow);
		result += "\n";
	}

	return result;
}

std::string core::commands::CORE_COMMAND_allLocalVars(core::User* who, core::CommandObject*) {
	std::string result;
	for (auto var : who->getAllVars()) {
		result += core::gprint(var, core::PrintColors::light_yellow);
		result += "\n";
	}

	return result;
}