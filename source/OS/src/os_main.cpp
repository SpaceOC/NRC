#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include "OS/other/vars.hpp"
#include "OS/other/commands.hpp"
#include "OS/other/pfs.hpp"
#include "OS/other/events.hpp"

void OSStart() {
	std::cout << "Welcome to <Your pseudo-OS name>!" << '\n';
	addCommands();
	addVariables();
	addFolderAndFiles();
	addEvents();
}