#ifndef NRC_COMMAND_STRUCTS_H_
#define NRC_COMMAND_STRUCTS_H_

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <variant>
#include "Core/utils/string_util.h"

namespace core {
	class User;

	struct CommandRules {
		size_t offParser = 1000; // is used to define after how many arguments to switch off the parser and save all other data in one argument
	};

	using SimpleCommand = std::string(*)(core::User*, core::CommandObject*);

	// Stores the function, and the minimum and maximum number of arguments
	struct ExtentedCommand {
		int minArgs, maxArgs;
		//std::function<std::string(core::User*, core::CommandObject*)> function;
		SimpleCommand function;
		core::CommandRules* rules = nullptr;
	};

	// Used for commandMap
	using CommandVariant = std::variant<SimpleCommand, ExtentedCommand>;

	// Stores the description and names of the arguments.
	struct CommandDescription {
		std::string description;
		std::vector<std::string> argsNames;
	};

	struct CommandObject {
		std::string name;
		std::vector<std::string> args = {};
		bool returnable = false;
		std::string whereOutput = "";

		friend std::ostream& operator<<(std::ostream& os, const CommandObject& obj) {
			return os << "{ " + obj.name + ", " << core::string_util::vectorToString(obj.args) << " }";
		}
	};

	using CustomRulesFunc = std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>;
}

#endif // NRC_COMMAND_STRUCTS_H_