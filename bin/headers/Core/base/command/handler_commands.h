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
#ifndef NRC_BASE_COMMAND_HANDLER_COMMANDS_H_
#define NRC_BASE_COMMAND_HANDLER_COMMANDS_H_

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

namespace core {
	// Stores a description and a function.
	struct CommandInfo {
		std::string description;
		std::function<void()> function;
	};

	// Stores the description, argument names, function, and the minimum and maximum number of arguments
	struct CommandWithArgsInfo {
		std::string description;
		int minArgs, maxArgs;
		std::vector<std::string> argsNames;
		std::function<void(std::vector<std::string>)> function;
	};

	// Stores the description and names of the arguments.
	struct CommandDescription {
		std::string description;
		std::vector<std::string> argsNames;
	};

	// Stores the name and arguments (for parsing, sendCommand).
	struct CommandObject {
		std::string name;
		std::vector<std::string> args = {};
	};

	class HandlerCommands {
		private:
			static inline std::map<std::string, CommandInfo> commandMap;
			static inline std::map<std::string, CommandWithArgsInfo> commandWithArgsMap;
			static inline std::string commandSeparator = "&/"; // Command separator.
		public:
			HandlerCommands();

			// Checks if the argument is a variable call.
			virtual bool thisVariable(const std::string& command) const;

			// Parses argument.
			virtual std::vector<CommandObject> parsing(const std::string& rawCommand) const;

			// Executes a command (if it exists and meets the required execution conditions). If the command is a variable call, it starts the variable.
			virtual void sendCommand(const core::CommandObject& command) const;

			static void setCommandSeparator(const std::string& commandSeparator);
			std::string getCommandSeparator();

			static void addCommand(const std::string& name, const std::string& description, const std::function<void()>& function);
			static void addCommand(const std::string& name, const CommandDescription& data, const std::function<void(std::vector<std::string>)>& function, int minArgs, int maxArgs);
			
			virtual std::map<std::string, CommandDescription> getCommand(const std::string& name) const;
			virtual std::map<std::string, CommandDescription> getAllCommands() const;
	};
}

#endif