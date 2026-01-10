#ifndef NRC_BASE_COMMAND_COMMANDS_HANDLER_H_
#define NRC_BASE_COMMAND_COMMANDS_HANDLER_H_

#include <string>
#include <functional>
#include <map>
#include "Core/command/command_parser.h"
#include "Core/users/user_permissions_enum.h"
#include "Core/command/command_structs.h"

namespace core {
	class User;
	class VariablesManager;

	class CommandsHandler {
		friend core::VariablesManager;
		private:
			std::map<std::string, CommandVariant> commandMap = {};
			std::map<std::string, CommandDescription> commandInfo = {};
			std::map<std::string, CustomRulesFunc> customRules = {};
			std::string commandSeparator = "&/"; // Command separator.
			CommandParser* parser = nullptr;
		protected:
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::UserPermissions permissions, const core::CommandObject& command);
			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			void sendCommand(const core::UserPermissions permissions, const core::CommandObject& command, std::string& str);
		public:
			CommandsHandler();
			CommandsHandler(CommandsHandler&) = delete;
			CommandsHandler(const CommandsHandler&&) = delete;
			CommandsHandler operator=(const CommandsHandler& right) = delete;

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
			const std::string& getCommandSeparator() { return commandSeparator; }

			void setCommandParser(CommandParser* newParser) { parser = newParser; }
			CommandParser* getParser() { return parser; }

			void addCommand(const std::string& name, const std::string& description, core::SimpleCommand function);
			void addCommand(const std::string& name, const CommandDescription& data, core::SimpleCommand function, int minArgs, int maxArgs, const CommandRules& rules);

			void addCustomRules(const std::string& id, const CustomRulesFunc& f);
			void deleteCustomRules(const std::string& id);

			void deleteCommand(const std::string& name);

			bool commandExists(const std::string& name);
			const CommandDescription& getCommand(const std::string& name);
			const std::map<std::string, CommandDescription>& getAllCommands();
	};

	extern CommandsHandler* commandsHandler();
}

#endif // NRC_BASE_COMMAND_COMMANDS_HANDLER_H_