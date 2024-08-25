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
#pragma once
#ifndef NRC_BASE_COMMAND_HANDLER_COMMANDS_H_
#define NRC_BASE_COMMAND_HANDLER_COMMANDS_H_

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

namespace core {
	struct CommandData {
		std::string description;
		std::function<void()> function;
	};

	struct ArgsCommandData {
		std::string description;
		int maxArgs;
		std::vector<std::string> argsNames;
		std::function<void(std::vector<std::string>)> function;
	};

	struct CommandBase {
		std::string description;
		std::vector<std::string> argsNames;
	};

	class handlerCommands {
		private:
			static inline std::map<std::string, CommandData> commandMap;
			static inline std::map<std::string, ArgsCommandData> commandWithArgsMap;
		public:
			handlerCommands();
			virtual bool thisVariable(const std::string& command) const;
			virtual std::vector<std::string> parsing(const std::string& command) const;
			virtual std::string realCommand(const std::string& badCommand) const;
			virtual void sendCommand(const std::string& command, const std::vector<std::string>& args) const;
			static void addCommand(const std::string& name, const std::string& description, const std::function<void()>& function);
			static void addCommand(const std::string& name, const CommandBase& data, const std::function<void(std::vector<std::string>)>& function, int args);
			virtual std::map<std::string, std::string> getCommand(const std::string& name) const;
			virtual std::map<std::string, CommandBase> getAllCommands() const;
	};
}

#endif