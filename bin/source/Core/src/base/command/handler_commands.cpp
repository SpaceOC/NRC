#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "Core/base/command/handler_commands.h"
#include "Core/extra/variables.h"
#include "Core/base/command/commands.h"
#include "Core/base/print.h"
#include "Core/base/users/user.h"
#include "Core/base/users/user_manager.h"
#include "Core/logs.h"
#include "Core/base/command/command_parser.h"

#ifndef NRC_WEB
#define STRING_TAB		"\t"
#else
#define STRING_TAB		"    "
#endif

core::HandlerCommands HC;
core::HandlerCommands* core::handlerCommands() {
	return &HC;
}

core::HandlerCommands::HandlerCommands() {}

bool core::HandlerCommands::thisVariable(const std::string& command) {
	return (command.substr(0, 1) == "%" && command.substr(command.length() - 1, command.length()) == "%");
}

std::vector<core::CommandObject> core::HandlerCommands::parsing(const std::string& raw, core::User* who, bool returnable) {
	if (raw.empty()) return {};
	std::vector<core::CommandObject> result;
	std::vector<std::string> splittedText = core::Utils::split(raw, ' ');
	splittedText.push_back(commandSeparator);
	std::vector<std::string> commandArgs;
	CommandRules* crules = nullptr;
	size_t argumentsN = 1;
	size_t fSizes = 0;
	std::string whereStopped = "start";
	try {
		for (size_t i = 0; i < splittedText.size(); i++) {
			if (splittedText.at(i) == commandSeparator) {
				whereStopped = "separator";
				std::string commandName = commandArgs.at(0);
				commandArgs.erase(commandArgs.begin());
				result.push_back({commandName, commandArgs, returnable});
				commandArgs = {};
				argumentsN = 1;
				splittedText.erase(splittedText.begin());
				if (splittedText.empty()) break;
			}
			else if (splittedText.at(i) == "->" && splittedText[i + 1] != commandSeparator) {
				whereStopped = "output";
				std::string commandName = commandArgs.at(0);
				size_t startIndex = raw.find("->", fSizes + 1);
				std::string fileName = raw.substr(startIndex + 3, raw.find(commandSeparator, startIndex));
				commandArgs.erase(commandArgs.begin());
				result.push_back({commandName, commandArgs, true, fileName});
				commandArgs = {};
				argumentsN = 1;
				for (size_t b = 0; b < splittedText.size(); b++) {
					if (splittedText.at(0) == commandSeparator) break;
					splittedText.erase(splittedText.begin());
					i = 0;
				}
				if (splittedText.empty() || splittedText.size() == 1) break;
				else if (splittedText.at(0) == commandSeparator) continue;
			}
			else if (crules && argumentsN >= crules->offParser) {
				whereStopped = "off parser";
				if (i > 0) {
					commandArgs.push_back(splittedText.at(0));
					splittedText.erase(splittedText.begin());
					--i;
				}
				size_t origIndex = raw.find(splittedText.at(i), fSizes);
				commandArgs.push_back(raw.substr(origIndex, raw.find(commandSeparator, fSizes)));
				std::string commandName = commandArgs.at(0);
				commandArgs.erase(commandArgs.begin());
				result.push_back({commandName, commandArgs, returnable});
				commandArgs = {};
				break;
			}

			if (splittedText.at(i)._Starts_with("\"") && splittedText.at(i) != commandSeparator) {
			//if (splittedText.at(i).find("\"") != std::string::npos && splittedText.at(i) != commandSeparator) {
				whereStopped = "commandArgs push (1)";
				if (!commandArgs.empty()) ++argumentsN;

				if (!crules || (crules && argumentsN < crules->offParser)) {
					int count = 0;
					std::string str = getStringInQ(splittedText, count);
					fSizes += str.length();

					for (int anotherCount = 0; anotherCount < count; anotherCount++) {
						splittedText.erase(splittedText.begin());
						i = -1;
					}
					commandArgs.push_back(str);
				}
			}
			else if (!splittedText.at(i)._Starts_with("\"") && splittedText.at(i) != commandSeparator) {
			//else if (splittedText.at(i).find("\"") == std::string::npos && splittedText.at(i) != commandSeparator) {
				whereStopped = "commandArgs push (2)";
				if (!commandArgs.empty()) ++argumentsN;

				if (!crules || (crules && argumentsN < crules->offParser)) {
					commandArgs.push_back(splittedText.at(i));
					fSizes += splittedText.at(i).length();

					if (commandArgs.size() == 1) {
						auto itArgs = commandWithArgsMap.find(commandArgs.at(0));
						if (itArgs != commandWithArgsMap.end()) {
							crules = itArgs->second.rules;
						}
					}

					if (splittedText.size() >= 2)
						splittedText.erase(splittedText.begin());
					i = -1;
				}
				else if (thisVariable(splittedText.at(i)) && core::Utils::split(raw, ' ').size() != 1) {
					std::string varName = splittedText.at(i).substr(1, splittedText.at(i).length() - 1);
					if (!who || (who && !who->varExists(varName))) {
						std::string str;
						core::systemVariablesManager()->start(varName, str);
						commandArgs.push_back(str);
					}
					else {
						std::string str;
						who->varFuncStart(varName, str);
						commandArgs.push_back(str);
					}
				}
			}
				/*
				else if (rawCommandData.substr(0, 1) == "\"" && !quoteOpened) {
					goodString += (rawCommandData.substr(0, 1) == "\"" ?
									rawCommandData.substr(1, rawCommandData.length()) :
									rawCommandData)
								+ " ";
					backString = rawCommandData;
					quoteOpened = true;
					if (rawCommandData.back() == '"') {
						anotherTemp.push_back(goodString.substr(0, goodString.length() - 2));
						goodString.erase();
						quoteOpened = false;
						if (anotherTemp.size() > 1) argumentsN++;
						continue;
					}
				}
				else if (rawCommandData.back() == '"' && quoteOpened) {
					goodString += rawCommandData + " ";
					anotherTemp.push_back(goodString.substr(0, goodString.length() - 2));
					goodString.erase();
					quoteOpened = false;
					if (anotherTemp.size() > 1) argumentsN++;
				}
				else if (quoteOpened && rawCommandData != commandSeparator && rawCommandData != backString) {
					goodString += rawCommandData + " ";
					backString = rawCommandData;
				}
				else {
					if (!goodString.empty()) anotherTemp.push_back(goodString); goodString.erase();
					anotherTemp.push_back(rawCommandData);
					auto itArgs = commandWithArgsMap.find(anotherTemp.at(0));
					if (itArgs != commandWithArgsMap.end() && !crules) {
						if (itArgs->second.rules) {
							crules = itArgs->second.rules;
							findRules = true;
						}
					}
					if (anotherTemp.size() > 1) argumentsN++;
				}
				*/
		}
			/*
			if (quoteOpened) {
				core::print("ERROR: One of the arguments was not closed (missing \" at the end of the argument)\n", core::PrintColors::red);
				return {};
			}
			*/
	}
	catch(const std::exception& e) {
		trace(core::PrintColors::red,
			"Parser error: ", e.what(), "\n",
			"Parser result: ", result, "\n",
			"Last commandArgs: ", commandArgs, "\n",
			"splittedText (Begin): ", core::Utils::split(raw, ' '), "\n",
			"splittedText (After): ", splittedText, "\n",
			"where: ", whereStopped
		);
		#ifdef NRC_WEB
		outputLog += "Parser error: " + std::string(e.what()) + "\nParser result: " +
			core::Utils::vectorToString(result) + "\nLast commandArgs: " +
			Utils::vectorToString(commandArgs) + "\nsplittedText (Begin): " +
			Utils::vectorToString(core::Utils::split(raw, ' ')) + "\nsplittedText (After): " +
			Utils::vectorToString(splittedText) + "\nwhere: " + whereStopped + "\n";
		#endif
	}
	//trace(core::PrintColors::yellow, result);
	return result;
}

void core::HandlerCommands::sendCommand(const core::CommandObject& command) {
	sendCommand(&core::userManager()->currentUserData(), command);
}

void core::HandlerCommands::sendCommand(const core::CommandObject& command, std::string& str) {
	sendCommand(&core::userManager()->currentUserData(), command, str);
}

void core::HandlerCommands::sendCommand(const core::Permissions permissions, const core::CommandObject& command) {
	core::User* user = new User("UNKNOWN_TEMP_USER_FOR_RUNNING_COMMANDS_OR_JS_CODE", permissions);
	sendCommand(user, command);
	delete user;
}

void core::HandlerCommands::sendCommand(const core::Permissions permissions, const core::CommandObject& command, std::string& str) {
	core::User* user = new User("UNKNOWN_TEMP_USER_FOR_RUNNING_COMMANDS_OR_JS_CODE", permissions);
	sendCommand(user, command, str);
	delete user;
}

void core::HandlerCommands::sendCommand(core::User* who, const core::CommandObject& command) {
    auto it = commandMap.find(command.name);
	std::string str, err;
	bool findCustomRule = std::any_of(customRules.begin(), customRules.end(), [&](std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)> func) -> bool {
		return func(command, who, str, err);
	});
	if (findCustomRule) {
		std::cout << str << '\n';
	}
	else if (!err.empty()) {
		core::print(err, core::PrintColors::red);
		core::print();
	}
    else if (it != commandMap.end() && command.args.empty()) {
		str = it->second.function(who, command.returnable);
    }
    else {
		auto itArgs = commandWithArgsMap.find(command.name);
    	if (itArgs != commandWithArgsMap.end()) {
			if (static_cast<size_t>(itArgs->second.maxArgs) < command.args.size()) {
				std::cout << "Too many arguments! Maximum number of command arguments: " << itArgs->second.maxArgs << '\n';
			}
			else if (static_cast<size_t>(itArgs->second.minArgs) > command.args.size()) {
				std::cout << "There are too few arguments! At least '" << itArgs->second.minArgs << "' is required" << '\n';
			}
			else {
				itArgs->second.function(who, command.args, false);
			}
		}
		else {
			std::cout << "Command not found" << '\n';
		}
	}
}

void core::HandlerCommands::sendCommand(core::User* who, const core::CommandObject& command, std::string& str) {
    auto it = commandMap.find(command.name);
	std::string err;
	bool findCustomRule = std::any_of(customRules.begin(), customRules.end(), [&](std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)> func) -> bool {
		return func(command, who, str, err);
	});
	if (findCustomRule)
		return;
	else if (!err.empty())
		str = err;
    else if (it != commandMap.end() && command.args.empty()) {
		str = it->second.function(who, command.returnable);
    }
    else {
		auto itArgs = commandWithArgsMap.find(command.name);
    	if (itArgs != commandWithArgsMap.end()) {
			if (static_cast<size_t>(itArgs->second.maxArgs) < command.args.size()) {
				str = "Too many arguments! Maximum number of command arguments: " + core::Utils::valueToString(itArgs->second.maxArgs) + "\n";
			}
			else if (static_cast<size_t>(itArgs->second.minArgs) > command.args.size()) {
				str = "There are too few arguments! At least '" + core::Utils::valueToString(itArgs->second.minArgs) + "' is required\n";
			}
			else {
				str = itArgs->second.function(who, command.args, command.returnable);
			}
		}
		else {
			str = "Command not found";
		}
	}
}

void core::HandlerCommands::addCommand(const std::string& name, const std::string& description, const std::function<std::string(core::User* who, bool returnable)>& function) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length()));
	temp += std::string(spacesToAdd, ' ');
	temp += std::string(STRING_TAB) + "  " + description;
	commandMap[name].function = function;
	commandMap[name].description = temp;
}

void core::HandlerCommands::addCommand(const std::string& name, const core::CommandDescription& data, const std::function<std::string(core::User* who, const std::vector<std::string>& args, bool returnable)>& function, int minArgs, int maxArgs, const CommandRules& rules) {
	std::string temp;
	int spacesToAdd = std::max(10, 46 - static_cast<int>(name.length() + data.argsNames.data()->length() + (7 * data.argsNames.size())));
	temp += std::string(spacesToAdd, ' ');
	temp += std::string(STRING_TAB) + "  " + data.description;
	commandWithArgsMap[name].minArgs = minArgs;
	commandWithArgsMap[name].maxArgs = maxArgs;
	commandWithArgsMap[name].argsNames = data.argsNames;
	commandWithArgsMap[name].function = function;
	commandWithArgsMap[name].description = temp;
	commandWithArgsMap[name].rules = new CommandRules(rules);
}

void core::HandlerCommands::addCustomRules(const std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>& f) {
	customRules.push_back(f);
}

void core::HandlerCommands::deleteCommand(const std::string& name) {
	if (!commandMap.count(name) && !commandWithArgsMap.count(name)) return;
	if (commandMap.count(name)) commandMap.erase(name);
	else commandWithArgsMap.erase(name);
}

std::map<std::string, core::CommandDescription> core::HandlerCommands::getCommand(const std::string& name) {
	if (commandMap.count(name))
		return {{name, {commandMap[name].description, {}}}};
	else if (commandWithArgsMap.count(name))
		return {{name, {commandWithArgsMap[name].description, commandWithArgsMap[name].argsNames}}};
	return {};
}

std::map<std::string, core::CommandDescription> core::HandlerCommands::getAllCommands() {
	if (commandMap.empty()) return {};
	std::map<std::string, core::CommandDescription> temp;
	for (auto commandData : commandMap) { 
		temp[commandData.first].description = commandData.second.description;
		temp[commandData.first].argsNames = {};
	}
	for (auto commandData : commandWithArgsMap) { 
		temp[commandData.first].description = commandData.second.description;
		temp[commandData.first].argsNames = commandData.second.argsNames;
	}
	return temp;
}