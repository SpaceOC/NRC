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

struct commandData {
	std::string description;
	std::function<void()> function;
};

struct commandBase {
	std::string name;
	std::map<std::string, std::vector<std::string>> args;
};

class handlerCommands {
	private:
		static inline std::map<std::string, commandData> commandMap;
	public:
		handlerCommands();
		virtual bool thisVariable(std::string command) const;
		virtual commandBase parsing(std::string& command) const;
		virtual void sendCommand(std::string command) const;
		void addCommand(std::string name, std::string description, std::function<void()> function) const;
		virtual std::map<std::string, std::string> getCommand(std::string name) const;
		virtual std::map<std::string, std::string> getAllCommands() const;
};
#endif