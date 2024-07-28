#pragma once
#ifndef HANDLER_COMMANDS
#define HANDLER_COMMANDS

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

struct commandData {
	std::string description;
	std::function<void()> function;
};

class handlerCommands {
	private:
		static inline std::map<std::string, commandData> commandMap;
	public:
		handlerCommands();
		virtual bool thisVariable(std::string command) const;
		virtual std::vector<std::string> parsing(std::string& command) const;
		virtual void sendCommand(std::string command) const;
		void addCommand(std::string name, std::string description, std::function<void()> function) const;
		virtual std::map<std::string, std::string> getCommand(std::string name) const;
		virtual std::map<std::string, std::string> getAllCommands() const;
};
#endif