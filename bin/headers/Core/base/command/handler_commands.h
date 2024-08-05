// Handler commands header
// by SpaceOC
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