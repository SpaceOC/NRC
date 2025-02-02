#ifndef NRC_BASE_COMMAND_HANDLER_COMMANDS_H_
#define NRC_BASE_COMMAND_HANDLER_COMMANDS_H_

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include "Core/base/users/permissions_enum_class.h"
#include "Core/base/command/commands.h"
#include "Core/settings.h"
#include "Core/base/utils.h"

namespace core {
	class User;
	class VariablesManager;

	struct CommandRules {
		size_t offParser = 1000; // is used to define after how many arguments to switch off the parser and save all other data in one argument
	};

	// Stores a description and a function.
	struct CommandInfo {
		std::string description;
		std::function<std::string(core::User* who, bool returnable)> function;
	};

	// Stores the description, argument names, function, and the minimum and maximum number of arguments
	struct CommandWithArgsInfo {
		std::string description;
		int minArgs, maxArgs;
		std::vector<std::string> argsNames;
		std::function<std::string(core::User* who, const std::vector<std::string>& args, bool returnable)> function;
		core::CommandRules* rules = nullptr;
	};

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
            return os << "{ " + obj.name + ", " << core::Utils::vectorToString(obj.args) << " }";
        }
	};

	class HandlerCommands {
		friend core::VariablesManager;
		private:
			std::map<std::string, CommandInfo> commandMap;
			std::map<std::string, CommandWithArgsInfo> commandWithArgsMap;
			std::vector<std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>> customRules;
			std::string commandSeparator = "&/"; // Command separator.
		protected:
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::Permissions permissions, const core::CommandObject& command);
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::Permissions permissions, const core::CommandObject& command, std::string& str);
		public:
			HandlerCommands();

			// Checks if the argument is a variable call.
			bool thisVariable(const std::string& command);

			// Parses argument.
			std::vector<CommandObject> parsing(const std::string& raw, core::User* who = NULL, bool returnable = false);

			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::CommandObject& command);
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::CommandObject& command, std::string& str);
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(core::User* who, const core::CommandObject& command);
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(core::User* who, const core::CommandObject& command, std::string& str);

			void setCommandSeparator(const std::string& newCommandSeparator) { commandSeparator = newCommandSeparator; }
			std::string getCommandSeparator() { return commandSeparator; }

			void addCommand(const std::string& name, const std::string& description, const std::function<std::string(core::User* who, bool returnable)>& function);
			void addCommand(const std::string& name, const CommandDescription& data, const std::function<std::string(core::User* who, const std::vector<std::string>& args, bool returnable)>& function, int minArgs, int maxArgs, const CommandRules& rules = {});

			void addCustomRules(const std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>& f);

			void deleteCommand(const std::string& name);

			std::map<std::string, CommandDescription> getCommand(const std::string& name);
			std::map<std::string, CommandDescription> getAllCommands();
	};

	extern HandlerCommands* handlerCommands();
}

#endif