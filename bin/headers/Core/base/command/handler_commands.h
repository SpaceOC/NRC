#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

class HandlerCommands {
private:
	static inline std::map<std::string, std::function<void(std::vector<std::string> vars)>> commandMap;
	static inline std::map<std::string, std::string> commandMapDescription;
public:
	HandlerCommands();
	virtual bool systemVariable(std::string command) const;
	virtual std::vector<std::string> parsing(std::string& command) const;
	virtual void sendCommand(std::string command) const;
	void addCommand(std::string commandName, std::string commandDescription, std::function<void(std::vector<std::string> vars)> commandFunction) const;
	virtual void getAllCommands() const;
};