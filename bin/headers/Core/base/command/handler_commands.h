#pragma once
#ifndef HANDLER_COMMANDS
#define HANDLER_COMMANDS

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

class handlerCommands {
	private:
		static inline std::map<std::string, std::function<void()>> commandMap;
		static inline std::map<std::string, std::string> commandMapDescription;
	public:
		handlerCommands();
		virtual bool systemVariable(std::string command) const;
		virtual std::vector<std::string> parsing(std::string& command) const;
		virtual void sendCommand(std::string command) const;
		void addCommand(std::string commandName, std::string commandDescription, std::function<void()> commandFunction) const;
		virtual void getAllCommands() const;
};
#endif