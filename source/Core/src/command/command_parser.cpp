#include "Core/command/command_parser.h"
#include "Core/command/handler_commands.h"
#include "Core/command/command_structs.h"
#include "Core/users/user.h"
#include "Core/other/variables.h"
#include "Core/print/print.h"

core::CommandParser::CommandParser() {}
core::CommandParser::~CommandParser() {}

std::vector<core::CommandObject> core::CommandParser::parse(const std::string& raw, bool parseVars, core::User* who) {
	if (raw.empty()) return {};

	std::vector<core::CommandObject> result;
	CommandObject* obj = nullptr;

	for (size_t i = 0; i < raw.size(); i++) {
		if (!__isCommandSeparator(raw, i) && !obj) {
			obj = __parseSingleCommand(raw, i, parseVars, who);
		}
		else if ((__isCommandSeparator(raw, i) || i == (raw.size() - 1)) && obj) {
			result.push_back(std::move(*obj));
			obj = nullptr;
			if (i != (raw.size() - 1)) {
				i += handlerCommands()->getCommandSeparator().size() - 1;
			}
		}
	}

	return result;
}

core::CommandObject* core::CommandParser::__parseSingleCommand(const std::string& raw, size_t startIndex, bool parseVars, core::User* who) {
	if (raw.empty() || (raw.size() <= startIndex)) return NULL;

	CommandObject* result = new CommandObject();
	std::string str;

	for (size_t i = startIndex; i < raw.size(); i++) {
		if (__isCommandSeparator(raw, i)) {
			break;
		}

		if (raw[i] != ' ') {
			str += raw[i];
		}
		if (i != 0 && raw[i] == '>' && raw[i - 1] == '-') {
			bool done = __setReturnableOnSingleCommand(result, raw, i);
			if (done) {
				break;
			}
		}

		if ((raw[i] == ' ' || (i + 1) >= raw.size()) && result->name.empty()) {
			result->name = str;
			str = "";
		}
		else if ((raw[i] == ' ' || (i + 1) >= raw.size()) && !result->name.empty()) {
			if (parseVars && who && handlerCommands()->thisVariable(str)) {
				std::string varName = str.substr(1, str.length() - 2);

				if (systemVariablesManager()->exists(varName) && systemVariablesManager()->getVariable(varName).type == VariableType::NAME) {
					systemVariablesManager()->start(varName, str);
				}
				else if (who->varExists(varName) && who->getVar(varName).type == VariableType::NAME) {
					who->varFuncStart(varName, str);
				}
			}

			result->args.push_back(str);
			str = "";
		}
		else if (raw[i] == '"') {
			if (i != 0 && raw[i - 1] == '\\') continue;

			size_t endIndex = i;
			str = __getStringInQ(raw, i, endIndex);

			if (endIndex != 0) i = endIndex;

			if (i >= (raw.size() - 1)) {
				if (result->name.empty()) {
					result->name = str;
				}
				else {
					result->args.push_back(str);
				}

				break;
			}
		}
	}

	return result;
}

std::string core::CommandParser::__getStringInQ(const std::string& original, size_t startIndex, size_t& end) {
	if (original.empty() || (original.size() <= startIndex)) {
		end = 0;
		return "";
	}
	std::string str;

	try {
		bool quoteOpened = false;
		for (size_t i = startIndex; i < original.size(); i++) {
			if (!quoteOpened && original[i] == '"') {
				++end;
				if (i != 0 && original[i - 1] == '\\') continue;

				quoteOpened = true;
				continue;
			}

			if (quoteOpened && original[i] == '"') {
				if (i != 0 && original[i - 1] == '\\') {
					str += original[i];
					end++;
					continue;
				}

				quoteOpened = false;
				break;
			}
			else if (quoteOpened && (original[i] != '"' || (i != 0 && original[i] == '"' && original[i - 1] == '\\'))) {
				str += original[i];
				++end;
			}
		}

		if (quoteOpened) {
			throw std::logic_error("The closing quotation mark is missing!");
		}
	}
	catch(const std::exception& e) {
		trace(core::PrintColors::red,
			"Error: ", e.what(), "\n",
			"original: ", original, "\n"
			"str: ", str
		);
		#ifdef NRC_WEB
		outputLog += "getStringInQ Error: " + std::string(e.what()) +
			"\noriginal " + original + "\nstr: " + str + "\n";
		#endif
	}
	return str;
}

std::pair<std::string, std::string> core::CommandParser::__getMapArgs(const std::string&) {
	return {};
}

bool core::CommandParser::__isCommandSeparator(const std::string& original, size_t startIndex) {
	if (original.empty() || (original.size() <= startIndex)) return false;

	std::string separator = handlerCommands()->getCommandSeparator();
	size_t originalIndex = startIndex;
	size_t end = startIndex + separator.size();

	for (size_t i = 0; i < separator.size(); i++) {
		if (originalIndex < end && original[originalIndex] != separator[i]) {
			return false;
		}
		else if (originalIndex >= end) {
			break;
		}

		++originalIndex;
	}

	return true;
}

bool core::CommandParser::__setReturnableOnSingleCommand(core::CommandObject* target, const std::string& raw, size_t startIndex) {
	if ((startIndex + 1) == raw.size() || (startIndex + 2) == raw.size()) return false;
	else if (raw[startIndex + 1] != ' ') return false;

	if (raw[startIndex + 2] != '"') {
		size_t spaces = 0;
		std::string str;

		for (size_t _ = startIndex + 2; _ < raw.size(); _++) {
			if (__isCommandSeparator(raw, _)) {
				if (raw[_ - 1] == ' ') --spaces;
				break;
			}

			if (raw[_] == ' ' && (_ + 1) != raw.size()) {
				++spaces;
				continue;
			}
			
			str += raw[_];
		}

		if (spaces > 0) return false;

		target->returnable = true;
		target->whereOutput = str;
	}
	else if (raw[startIndex + 2] == '"') {
		size_t endIndex = startIndex;
		std::string str = __getStringInQ(raw, startIndex, endIndex);

		if (endIndex == 0) return false;

		target->returnable = true;
		target->whereOutput = str;
	}

	return true;
}