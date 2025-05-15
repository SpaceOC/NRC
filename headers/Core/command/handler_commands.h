#ifndef NRC_BASE_COMMAND_HANDLER_COMMANDS_H_
#define NRC_BASE_COMMAND_HANDLER_COMMANDS_H_

#include <string>
#include <functional>
#include "Core/command/commands.h"
#include "Core/settings.h"
#include "Core/other/utils.h"
#include "Core/command/command_parser.h"
#include "Core/users/user_permissions_enum.h"

namespace core {
	class User;
	class VariablesManager;
	struct CommandRules;
	struct CommandInfo;
	struct CommandWithArgsInfo;
	struct CommandDescription;
	struct CommandObject;

	class HandlerCommands {
		friend core::VariablesManager;
		private:
			std::map<std::string, CommandInfo> commandMap;
			std::map<std::string, CommandWithArgsInfo> commandWithArgsMap;
			std::vector<std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>> customRules;
			std::string commandSeparator = "&/"; // Command separator.
			CommandParser* parser;
		protected:
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::UserPermissions permissions, const core::CommandObject& command);
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::UserPermissions permissions, const core::CommandObject& command, std::string& str);
		public:
			HandlerCommands();

			// Checks if the argument is a variable call.
			bool thisVariable(const std::string& command);

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

			void setCommandParser(CommandParser* newParser) { parser = newParser; }
			CommandParser* getParser() { return parser; }

			void addCommand(const std::string& name, const std::string& description, const std::function<std::string(core::User*, core::CommandObject*)>& function);
			void addCommand(const std::string& name, const CommandDescription& data, const std::function<std::string(core::User*, core::CommandObject*)>& function, int minArgs, int maxArgs, const CommandRules& rules);

			void addCustomRules(const std::function<bool(const core::CommandObject& c, core::User* who, std::string& ret, std::string& err)>& f);

			void deleteCommand(const std::string& name);

			std::map<std::string, CommandDescription> getCommand(const std::string& name);
			std::map<std::string, CommandDescription> getAllCommands();
	};

	extern HandlerCommands* handlerCommands();
}

#endif